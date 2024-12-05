#include "LightShowSocket.h"
#include "Arduino.h"
#include "ArduinoJson.h"

void LightShowSocket::registerClient() {
  DynamicJsonDocument sendDoc(1024);
  JsonArray registerArray = sendDoc.to<JsonArray>();

  registerArray.add(EVENT_CLIENT_REGISTER);
  registerArray.add(_id);

  String sendPayload;
  serializeJson(sendDoc, sendPayload);

  client.sendEVENT(sendPayload);
}

int *LightShowSocket::parseNotes(const String &noteStr, int &notesSize) {
  int *numArray = NULL;                  // Pointer to int array
  char charArray[noteStr.length() + 1];  // Create a char array to hold the string

  noteStr.toCharArray(charArray, sizeof(charArray));  // Convert String to char array

  char *token = strtok(charArray, ",");  // Get the first token

  int i = 0;
  while (token != NULL) {
    numArray = (int *)realloc(numArray, (i + 1) * sizeof(int));  // Dynamically allocate memory for the array
    numArray[i++] = atoi(token);                                 // Convert token to integer and store in array
    token = strtok(NULL, ",");                                   // Get the next token
  }
  notesSize = i;
  return numArray;
}


void LightShowSocket::socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case sIOtype_DISCONNECT:
      Serial.printf("[socket] Disconnected");
      break;
    case sIOtype_CONNECT:
      Serial.printf("[socket] Connected to url: %s\n", payload);
      client.send(sIOtype_CONNECT, "/");
      delay(500);
      registerClient();
      break;
    case sIOtype_EVENT:
      {
        DeserializationError error = deserializeJson(doc, payload, length);
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
          break;
        }
        const String eventName = doc[0];

        if (eventName == EVENT_NOTE_ON || eventName == EVENT_NOTE_OFF) {
          JsonArray notes = doc[1].as<JsonArray>();
          if (eventName == EVENT_NOTE_ON && onMultiNoteOnHandler != NULL) {
            notesOn(notes, onMultiNoteOnHandler);
            break;
          }
          int noteIndex = eventName == EVENT_NOTE_ON ? isNoteInRange(notes) : isSingleNoteInRange(doc[2]);
          if (noteIndex == -1) {
            return;
          }

          if (eventName == EVENT_NOTE_ON) {
            long length = doc[2];
            int velocity = doc[3];
            onNoteOnHandler(noteIndex, length, velocity);
          } else {
            onNoteOffHandler(noteIndex);
          }
          break;
        }
        if (eventName == EVENT_NOTES_MAP) {
          const String clientId = doc[1];
          bool isPlaying = doc[5];
          if (clientId != _id) {
            break;
          }

          int notesSize = 0;
          int *notes = parseNotes(doc[3], notesSize);
          _notesSize = notesSize;
          _notes = notes;
          if (onMapNotesHandler != NULL) {
            onMapNotesHandler(notes, notesSize, isPlaying);
          }
          break;
        }

        if (eventName == EVENT_TRACK_START && onTrackStartHandler != NULL) {
          onTrackStartHandler();
          break;
        }
        if (eventName == EVENT_TRACK_END && onTrackEndHandler != NULL) {
          onTrackEndHandler();
          break;
        }
        if (onOTAEventHandler != NULL && (eventName == EVENT_OTA_ON || eventName == EVENT_OTA_OFF)) {
          const String clientId = doc[1];
          if (clientId != _id) {
            break;
          }
          onOTAEventHandler(eventName == EVENT_OTA_ON);
          break;
        }
        if (onClientEnableHandler != NULL && (eventName == EVENT_CLIENT_ENABLE || eventName == EVENT_CLIENT_DISABLE)) {
          const String clientId = doc[1];
          if (clientId != _id) {
            break;
          }
          onClientEnableHandler(eventName == EVENT_CLIENT_ENABLE);
          break;
        }
      }
      break;
    case sIOtype_ACK:
      hexdump(payload, length);
      break;
    case sIOtype_ERROR:
      hexdump(payload, length);
      break;
    case sIOtype_BINARY_EVENT:

      hexdump(payload, length);
      break;
    case sIOtype_BINARY_ACK:
      hexdump(payload, length);
      break;
  }
}

void LightShowSocket::listen() {
  client.loop();
}

void LightShowSocket::setup(const char *id, const char *host, uint16_t port) {
  _id = id;
  client.onEvent([this](socketIOmessageType_t type, uint8_t *payload, size_t length) {
    this->socketIOEvent(type, payload, length);
  });

  client.begin(host, port, "/socket.io/?EIO=4");
}

void LightShowSocket::onTrackStart(EventHandler handler) {
  onTrackStartHandler = handler;
}

void LightShowSocket::onTrackEnd(EventHandler handler) {
  onTrackEndHandler = handler;
}

void LightShowSocket::onTrackLoad(EventHandler handler) {
  onTrackLoadHandler = handler;
}

void LightShowSocket::onNoteOn(NoteOnEventHandler handler) {
  onNoteOnHandler = handler;
}

void LightShowSocket::onMultiNoteOn(MultiNoteOnEventHandler handler) {
  onMultiNoteOnHandler = handler;
}

void LightShowSocket::onNoteOff(NoteEventHandler handler) {
  onNoteOffHandler = handler;
}

void LightShowSocket::onMapNotes(MapNotesEventHandler handler) {
  onMapNotesHandler = handler;
}

void LightShowSocket::onOTAEvent(OTAEventHandler handler) {
  onOTAEventHandler = handler;
}

void LightShowSocket::onClientEnable(ClientEnableHandler handler) {
  onClientEnableHandler = handler;
}

int LightShowSocket::isNoteInRange(JsonArray notes) {
  for (int x = 0; x < _notesSize; x++) {
    for (JsonVariant note : notes) {
      if (_notes[x] == note.as<int>()) {
        return x;
      }
    }
  }
  return -1;
}

int LightShowSocket::isSingleNoteInRange(int note) {
  for (int x = 0; x < _notesSize; x++) {
    if (_notes[x] == note) {
      return x;
    }
  }
  return -1;
}

void LightShowSocket::notesOn(JsonArray notes, MultiNoteOnEventHandler handler) {
  for (int x = 0; x < _notesSize; x++) {
    for (JsonVariant note : notes) {
      if (_notes[x] == note.as<int>()) {
        handler(x);
      }
    }
  }
}

int LightShowSocket::getNotesSize() {
  return _notesSize;
}
