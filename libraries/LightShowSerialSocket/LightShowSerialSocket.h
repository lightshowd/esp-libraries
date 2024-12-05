#ifndef LightShowSocket_h
#define LightShowSocket_h

#include "Arduino.h"
#include "ArduinoJson.h"

#define EVENT_NOTE_ON "note:on"
#define EVENT_NOTE_OFF "note:off"
#define EVENT_TRACK_START "track:start"
#define EVENT_TRACK_LOAD "track:load"
#define EVENT_TRACK_END "track:end"
#define EVENT_CLIENT_REGISTER "client:register"
#define EVENT_NOTES_MAP "notes:map"
#define EVENT_SERVER_CONNECT "server:connect"

class LightShowSerialSocket {
  typedef std::function<void()> EventHandler;
  typedef std::function<void(int note)> NoteEventHandler;
  typedef std::function<void(int note, long length, int velocity)> NoteOnEventHandler;
  typedef std::function<void(int note, long length, int velocity, int *sameNotes, int sameNotesSize)> MultiNoteOnEventHandler;
  typedef std::function<void(int *notes, int length)> MapNotesEventHandler;
private:
  const char *_id;
  int *_notes;
  int _notesSize;
  DynamicJsonDocument doc;
  EventHandler onTrackStartHandler;
  EventHandler onTrackEndHandler;
  EventHandler onTrackLoadHandler;
  NoteOnEventHandler onNoteOnHandler;
  NoteEventHandler onNoteOffHandler;
  MapNotesEventHandler onMapNotesHandler;
  MultiNoteOnEventHandler onMultiNoteOnHandler;

  int *parseNotes(const String &noteStr, int &notesSize);

public:
  LightShowSerialSocket()
    : doc(1024) {};

  void setup(const char *id);
  void registerClient();
  void listen();
  void onTrackStart(EventHandler handler);
  void onTrackEnd(EventHandler handler);
  void onTrackLoad(EventHandler handler);
  void onMapNotes(MapNotesEventHandler handler);
  void onNoteOn(NoteOnEventHandler handler);
  void onMultiNoteOn(MultiNoteOnEventHandler handler);
  void onNoteOff(NoteEventHandler handler);
  int isNoteInRange(JsonArray notes);
  int getNotesSize();
};




#endif