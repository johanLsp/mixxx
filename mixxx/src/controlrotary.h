#ifndef CONTROLROTARY_H
#define CONTROLROTARY_H

class MidiObject;
#include "controlobject.h"
#include "controlpotmeter.h"
#include "defs.h"
#include "midiobject.h"
#include <algorithm>

class ControlRotary : public ControlPotmeter
{
  Q_OBJECT
 private:
  timeb oldtime;
  FLOAT_TYPE counter;
  static const char graycodetable[256];
 public:
  short direction;
  ControlRotary(char*, int, MidiObject*);
  void updatecounter(int, int SRATE);
  short sign(short);
  void midiEvent(int);
 public slots:
  void slotSetPosition(int);
};

#endif
