#include "../include/Player.h"


Player::Player() : uid(0), position{0, 0} {}

Player::Player(int id, float x, float y, char col){
    this->position.x = x;
    this->position.y = y;
    // this->connectToServer(PORT);
    this->uid = id;
    this->color = col;
}

Player:: Player(int id, Point pos, char col){
    this->uid =  id;
    this->position = pos;
    this->color = col;

}


Player::~Player(){
}
void Player::moveRight(){
    this->position.x += x_vel;
}
void Player::moveDown(){
    this->position.y += y_vel;
}

void Player::moveUp(){
    this->position.y -= y_vel;
}

void Player::moveLeft(){
    this->position.x -= x_vel;
}
