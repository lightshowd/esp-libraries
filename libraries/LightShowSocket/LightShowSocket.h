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
#define EVENT_OTA_ON "ota:on"
#define EVENT_OTA_OFF "ota:off"
#define EVENT_CLIENT_ENABLE "client:enable"
#define EVENT_CLIENT_DISABLE "client:disable"

class LightShowSocket {
  typedef std::function<void()> EventHandler;
  typedef std::function<void(int note)> NoteEventHandler;
  typedef std::function<void(int note, long length, int velocity)> NoteOnEventHandler;
  typedef std::function<void(int note)> MultiNoteOnEventHandler;
  typedef std::function<void(int *notes, int length, bool isTrackPlaying)> MapNotesEventHandler;
  typedef std::function<void(bool isOn)> OTAEventHandler;
  typedef std::function<void(bool enable)> ClientEnableHandler;
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
  MultiNoteOnEventHandler onMultiNoteOnHandler;
  OTAEventHandler onOTAEventHandler;
  ClientEnableHandler onClientEnableHandler;

  void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length);
  void registerClient();
  int *parseNotes(const String &noteStr, int &notesSize);

public:
  LightShowSocket()
    : client(), doc(1024) {};

  void setup(const char *id, const char *host, uint16_t port);
  void listen();
  void onTrackStart(EventHandler handler);
  void onTrackEnd(EventHandler handler);
  void onTrackLoad(EventHandler handler);
  void onMapNotes(MapNotesEventHandler handler);
  void onNoteOn(NoteOnEventHandler handler);
  void onMultiNoteOn(MultiNoteOnEventHandler handler);
  void onNoteOff(NoteEventHandler handler);
  void onOTAEvent(OTAEventHandler handler);
  void onClientEnable(ClientEnableHandler handler);
  int isNoteInRange(JsonArray notes);
  int isSingleNoteInRange(int note);
  void notesOn(JsonArray notes, MultiNoteOnEventHandler handler);
  int getNotesSize();
};




#endif