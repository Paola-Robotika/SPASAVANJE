#pragma once
#include "helper.h"

void RobotLine::radionica() {
  // Uvođenje varijable lopta s kojom mjenjamo ponašanje robota ovisno dali drži robot loptu ili ne
  if (setup()) {
    lopta = false;
    armOpen();
  }

  // Ukoliko robot NE nosi loptu ponašat će se prema sljedećim naredbama
  if (lopta == false) {
    findingBall();
  }

  // Ukoliko robot NOSI loptu ponašat će se prema sljedećim naredbama
  else {
    carryingBall();
  }
}
