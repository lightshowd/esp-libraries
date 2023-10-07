#ifndef LightShowSocket_h
#define LightShowSocket_h

#include "Arduino.h"
#include "WebSocketsClient.h"
#include "SocketIOclient.h"
#include "ArduinoJson.h"

#define EVENT_NOTE_ON "note:on"
#define EVENT_NOTE_OFF "note:off"
#define EVENT_TRACK_START "track:start"
#define EVENT_TRACK_LOAD "track:load"
#define EVENT_TRACK_END "track:end"
#define EVENT_CLIENT_REGISTER "client:register"
#define EVENT_NOTES_MAP "notes:map"

class LightShowSocket {
  typedef std::function<void()> EventHandler;
  typedef std::function<void(int note)> NoteEventHandler;
  typedef std::function<void(int note, long length, int velocity)> NoteOnEventHandler;
  typedef std::function<void(int *notes, int length)> MapNotesEventHandler;
private:
  const char *_id;
  int *_notes;
  int _notesSize;
  SocketIOclient client;
  DynamicJsonDocument doc;
  EventHandler onTrackStartHandler;
  EventHandler onTrackEndHandler;
  EventHandler onTrackLoadHandler;
  NoteOnEventHandler onNoteOnHandler;
  NoteEventHandler onNoteOffHandler;
  MapNotesEventHandler onMapNotesHandler;

  void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length);
  void registerClient();
  int *parseNotes(const String &noteStr, int &notesSize);

public:
  LightShowSocket()
    : client(), doc(1024){};

  void setup(const char *id, const char *host, uint16_t port);
  void listen();
  void onTrackStart(EventHandler handler);
  void onTrackEnd(EventHandler handler);
  void onTrackLoad(EventHandler handler);
  void onMapNotes(MapNotesEventHandler handler);
  void onNoteOn(NoteOnEventHandler handler);
  void onNoteOff(NoteEventHandler handler);
  int isNoteInRange(int note);
};




#endif