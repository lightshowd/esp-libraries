#include "LightShowSerialSocket.h"
#include "Arduino.h"
#include "ArduinoJson.h"

void LightShowSerialSocket::registerClient() {
  DynamicJsonDocument sendDoc(1024);
  JsonArray registerArray = sendDoc.to<JsonArray>();

  registerArray.add(EVENT_CLIENT_REGISTER);
  registerArray.add(_id);

  if (Serial.availableForWrite()) {
    serializeJson(sendDoc, Serial);
    Serial.println();
  } else {
    Serial.println("Serial not available for write");
  }
}

int *LightShowSerialSocket::parseNotes(const String &noteStr, int &notesSize) {
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

void LightShowSerialSocket::listen() {
  if (Serial.available()) {

    DeserializationError err = deserializeJson(doc, Serial);
    if (err) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(err.c_str());
      return;
    }

    const String eventName = doc[0];
    if (eventName == EVENT_NOTE_ON || eventName == EVENT_NOTE_OFF) {
      JsonArray notes = doc[1].as<JsonArray>();
      int noteIndex = isNoteInRange(notes);
      if (noteIndex == -1) {
        return;
      }
      if (eventName == EVENT_NOTE_ON) {
        long length = doc[2];
        int velocity = doc[3];
        if (onMultiNoteOnHandler != NULL) {
          int sameNotesSize = 0;
          int *sameNotes = parseNotes(doc[4], sameNotesSize);
          onMultiNoteOnHandler(noteIndex, length, velocity, sameNotes, sameNotesSize);
        } else {
          onNoteOnHandler(noteIndex, length, velocity);
        }
      } else {
        onNoteOffHandler(noteIndex);
      }
    } else if (eventName == EVENT_NOTES_MAP) {
      const String clientId = doc[1];
      if (clientId != _id) {
        return;
      }

      int notesSize = 0;
      int *notes = parseNotes(doc[3], notesSize);
      _notesSize = notesSize;
      _notes = notes;
      if (onMapNotesHandler != NULL) {
        onMapNotesHandler(notes, notesSize);
      }
    }

    else if (eventName == EVENT_TRACK_START && onTrackStartHandler != NULL) {
      onTrackStartHandler();
    } else if (eventName == EVENT_TRACK_END && onTrackEndHandler != NULL) {
      onTrackEndHandler();
    } else if (eventName == EVENT_SERVER_CONNECT) {
      registerClient();
    }
  }
}

void LightShowSerialSocket::setup(const char *id) {
  _id = id;
  // Serial.println("Serial Socket Connected");
}

void LightShowSerialSocket::onTrackStart(EventHandler handler) {
  onTrackStartHandler = handler;
}

void LightShowSerialSocket::onTrackEnd(EventHandler handler) {
  onTrackEndHandler = handler;
}

void LightShowSerialSocket::onTrackLoad(EventHandler handler) {
  onTrackLoadHandler = handler;
}

void LightShowSerialSocket::onNoteOn(NoteOnEventHandler handler) {
  onNoteOnHandler = handler;
}

void LightShowSerialSocket::onMultiNoteOn(MultiNoteOnEventHandler handler) {
  onMultiNoteOnHandler = handler;
}

void LightShowSerialSocket::onNoteOff(NoteEventHandler handler) {
  onNoteOffHandler = handler;
}

void LightShowSerialSocket::onMapNotes(MapNotesEventHandler handler) {
  onMapNotesHandler = handler;
}

int LightShowSerialSocket::isNoteInRange(JsonArray notes) {
  for (int x = 0; x < _notesSize; x++) {
    for (JsonVariant note : notes) {
      if (_notes[x] == note.as<int>()) {
        return x;
      }
    }
  }

  return -1;
}

int LightShowSerialSocket::getNotesSize() {
  return _notesSize;
}
