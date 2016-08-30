#include "Objects.h"

//------------------------Pikachu--------------------------------------------
Pikachu::Pikachu(const Point& position, int size, PlayerCommands* commands) :
        Player(position, Parameters(size), commands), size(size) {

}

PlayerTypeParameters Pikachu::Parameters(int scale) {
    double speed_divider = 100;
    PlayerTypeParameters parameters = {TX_YELLOW, scale/2, scale, scale, scale/speed_divider};
    return parameters;
}

void Pikachu::Update() {
    Player::Update();
    if (commands->hit != ShootType::NONE  && time >= 50) {
        time = 0;
         switch (commands->hit){
             case ShootType::PUNCH:
                 ShootBullet(new IronTale(size));
                 break;
             case ShootType::SHOT:
                 ShootBullet(new Discharge(size));
                 break;
         }
    }
}

//------------------------Discharge----------------------------------------
Discharge::Discharge(double size) : Bullet(size/20, TX_YELLOW, size/10, size/60) {
   // cout << "Discharge size:" << size << endl;
}

//------------------------IronTale----------------------------------------
IronTale::IronTale(double size) : Bullet(size/10, TX_YELLOW, size/20, size/60) {
   // cout << "IronTale size:" << size << endl;
}

//------------------------Charmander-----------------------------
Charmander::Charmander(const Point& position, int size, PlayerCommands* commands)
        : Player(position, Parameters(size), commands){

}

PlayerTypeParameters Charmander::Parameters(int scale) {
    double speed_divider = 100;
    PlayerTypeParameters parameters = {TX_RED, scale/2, scale, scale, scale/speed_divider};
    return parameters;
}

void Charmander::Update() {
    Player::Update();
    if (commands->hit != ShootType::NONE)
        switch (commands->hit){
            case ShootType::PUNCH:
                ShootBullet(new FireHit(size));
                break;
            case ShootType::SHOT:
                ShootBullet(new FireBall(size));
                break;
        }
}

//------------------------FireBall-------------------------------

FireBall::FireBall(double size) : Bullet(size/10, TX_RED, size/10, size/40) {

}

//------------------------FireHit---------------------------------

FireHit::FireHit(double size) : Bullet(size/10, TX_RED, size/20, size/20) {

}