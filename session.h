#ifndef SESSION_H
#define SESSION_H

class session
{
public:
    session(int _intensity=1, int _session=0);

    // Getters
    int getPreferedIntensity();
    int getCurrentSession();

    // Setters
    void setPreferedIntensity(int inten);

private:
    // Session id
    int currentSessionID; //0 = alpha, 1 = delta, 2 = theta, 3 = MET
    //intensity saved by user
    int preferedIntensity; //can be a value between 1 and 8

};

#endif // SESSION_H
