#include "session.h"


session::session(int _session, int _intensity)

{
    currentSessionID = _session;
    preferedIntensity = _intensity;
}


// Getters

int session::getPreferedIntensity() { return preferedIntensity; }
int session::getCurrentSession() { return currentSessionID; }



// Setters
void session::setPreferedIntensity(int inten) { preferedIntensity = inten; }
