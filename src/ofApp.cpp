#include "ofApp.h"
// Yukai Yang 2018
// puts the game together

//--------------------------------------------------------------
void ofApp::setup(){
    score = 0;
    game_state = State::START;
    player.setup();
    ofBackground(54, 54, 54, 100);  // gray background
    verdana14.load("font/verdana.ttf", 14, true, true);  /* from font examples */
    verdana14.setLineHeight(24.0f);
    verdana14.setLetterSpacing(1.2);
    background.load("images/skybkgedit.png"); // WIP
    
    
    ofImage temp;
    temp.load("images/skull.png");
    
    enemySpawner = new Emitter(new SpriteSystem);
    enemySpawner->setChildSize(20,20); // SPRITE SIZE set to smaller than actual sprite
    enemySpawner->setChildImage(temp);
    enemySpawner->setPosition(ofVec3f(ofGetWindowWidth(),ofGetWindowHeight()/2));
    enemySpawner->setVelocity(ofVec3f(-150,0,0));
    enemySpawner->setLifespan(10000);
    enemySpawner->setValue(100);
    enemySpawner->setRate(1.3);
    enemySpawner->start();
    
    enemySpawner2 = new Emitter(new SpriteSystem);
    enemySpawner2->setChildSize(20,20);
    enemySpawner2->setChildImage(temp);
    enemySpawner2->setPosition(ofVec3f(ofGetWindowWidth(),ofGetWindowHeight()/3 * 2));
    enemySpawner2->setVelocity(ofVec3f(-150,0,0));
    enemySpawner2->setLifespan(10000);
    enemySpawner2->setValue(100);
    enemySpawner2->setRate(1.3);
    
    /*
    for (int i=0 ; i < 5; i++){
        Emitter *tmp = new Emitter(new SpriteSystem);
        enemySpawner2->setChildSize(20,20);
        enemySpawner2->setChildImage(temp);
        enemySpawner2->setPosition(ofVec3f(ofGetWindowWidth(),ofGetWindowHeight()/2));
        enemySpawner2->setVelocity(ofVec3f(-400,0,0));
        enemySpawner2->setLifespan(10000);
        enemySpawner2->setValue(100);
        enemySpawner2->setRate(2);
    }
    */
    heart1.load("images/health.png");
    heart2.load("images/health.png");
    heart3.load("images/health.png");
    
    level = 1;
    timer = 0;
    gameTimer = 0;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if (game_state == State::START){    // Do nothing until game begins
    } else if (game_state == State::GAME){
        
        if (timer > 0 ){
            timer --;
        }
        else{
            player.status = Player::State::VULNERABLE;
        }
            
        player.update();
        checkCollisions();

        if (level == 1){
            enemySpawner->sys->setPath(Sprite::Path::SIN);
            enemySpawner2->sys->setPath(Sprite::Path::COS);
            
            if (score > 1000){
                level = 2;
                enemySpawner->stop();
                enemySpawner->setPosition(ofVec3f(ofGetWindowWidth(),ofGetWindowHeight()/3));
                enemySpawner->start();
                enemySpawner2->start();
            }
        }
        else if (level == 2){
            if (score > 2000){
                level = 3;
                enemySpawner->setVelocity(ofVec3f(-250,0,0));
                enemySpawner->setRate(2);
                enemySpawner2->setVelocity(ofVec3f(-250,0,0));
                enemySpawner2->setRate(2);
            }
        }
        else if(level == 3){
            if (score > 5000) {
                level = 4;
                enemySpawner->setRate(0);
                enemySpawner2->setRate(0);
                enemySpawner->sys->setPath(Sprite::Path::STR);
                enemySpawner2->sys->setPath(Sprite::Path::STR);
                gameTimer = 160;
            }
        }
        else if(level == 4) {
            if (gameTimer > 0){
                gameTimer--;
            }
            else {
                level = 5;
                enemySpawner->setRate(10);
                enemySpawner2->setRate(10);
            }
        }
        else if (level ==5){
            enemySpawner->setVelocity(ofVec3f(-400,ofRandom(-100,100)));
            enemySpawner2->setVelocity(ofVec3f(-400,ofRandom(-100,100)));

        }
        handleExplosions();
        enemySpawner->update();
        enemySpawner2->update();
        
        
    } else if (game_state == State::PAUSE) {
        
    } else if (game_state== State::GAME_OVER){
    
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);

    if (game_state == State::START){    // Initial Screne
        //ofSetColor(225);
        verdana14.drawString("Witch Game",500,320);
        verdana14.drawString("Press Spacebar", 482, 380);
    } else if (game_state == State::GAME){
        

        background.draw(0,0);
        
        heart1.draw(50,50);
        heart2.draw(80,50);
        heart3.draw(110,50);
        verdana14.drawString(to_string(score),1000,50);

        if (player.status == Player::State::INVULNERABLE){
            if (timer%10 == 0)
                player.draw();
        }
        else
            player.draw();
        
        enemySpawner->draw();
        enemySpawner2->draw();
        
        /* 2d explosion */
        for (int i=0; i < explosions.size(); i++){
            explosions[i]->draw();
        }

        
    } else if (game_state == State::PAUSE){
        background.draw(0,0);
        player.draw();
        enemySpawner->draw();
        
        verdana14.drawString("Paused",520,320);
        verdana14.drawString("Press Spacebar", 482, 380);
    
    } else if (game_state== State::GAME_OVER){
        background.draw(0,0);
        player.draw();
        enemySpawner->draw();
        
        verdana14.drawString("You died :(",520,320);
        verdana14.drawString("Press Spacebar", 482, 380);
    }

}

//--------------------------------------------------------------

void ofApp::checkCollisions() {

    // remove all sprites emitted within a radius equal to the max
    // distance sprite can travel in one frame.
    //
    float dist = enemySpawner->maxDistPerFrame()+enemySpawner->childWidth;

    vector<Missile>::iterator m = player.missiles.begin();
    vector<Missile>::iterator tmp;
    int numRemoved;
    // Remove Missiles and that collide
    while(m != player.missiles.end()){
        /*
        if(enemySpawner->sys->removeNear(m->sprite.trans, dist)){
            tmp = player.missiles.erase(m);
            score += enemySpawner->value;
            m = tmp;
        }
        */
        
        // Check collisions and explode
        numRemoved = enemySpawner->sys->removeNear(m->sprite.trans, dist, collisionLocations);
        numRemoved += enemySpawner2->sys->removeNear(m->sprite.trans, dist, collisionLocations);
        if(numRemoved > 0){
            explode(numRemoved, collisionLocations);
            tmp = player.missiles.erase(m);
            score += enemySpawner->value;
            m = tmp;
        }
        else
            m++;
    }
    if(enemySpawner->sys->removeNear(player.sprite.trans,dist) ||
        enemySpawner2->sys->removeNear(player.sprite.trans,dist)){
        if (player.status == Player::State::INVULNERABLE){
        }
        else {
            player.takeDamage();
            switch (player.health){
                    case 0:
                        heart1.load("images/EmptyHealth.png");
                        game_state = State::GAME_OVER;
                        break;
                    case 1:
                        heart2.load("images/EmptyHealth.png");
                        timer = 120; // 120 frames = 2 seconds
                        break;
                    case 2:
                        heart3.load("images/EmptyHealth.png");
                        timer = 120; // 120 frames = 2 seconds
                        break;
            }
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::explode(int numRemoved, vector<ofVec2f> &collisionLocations){
    /* create new explosion */
    for (int i=0; i < numRemoved; i++) {
        ParticleEmitter *e = new ParticleEmitter();
        e->setOneShot(true);
        e->setEmitterType(RadialEmitter);
        e->setGroupSize(50);
        e->setLifespan(6);
        e->setVelocity(ofVec3f(1000, 0, 0));
        e->setParticleRadius(2);
        e->sys->addForce(new GravityForce(ofVec3f(0,200,0)));
        
        //cout <<endl <<i <<" : ("<<collisionLocations[i].x <<"," << collisionLocations[i].y<<")"<<endl;
        e->position = collisionLocations[0]; // assign first location to position because next location will
        e->start();                          // automatically be brought forward.
        explosions.push_back(e);
        collisionLocations.erase(collisionLocations.begin());
    }
}
//--------------------------------------------------------------
void ofApp::handleExplosions(){
    /*2d explosion*/
    ParticleEmitter *tmp;
    vector<ParticleEmitter*>::iterator e = explosions.begin();
    vector<ParticleEmitter*>::iterator pos;
    while(e != explosions.end()){
        (*e)->update();
        /* delete explosion 1 second after particles are gone */
        if((*e)->age()!= -1 && (*e)->age() > (*e)->lifespan+100) { // pointer to pointer to particle emitter
            tmp = *e;
            pos = explosions.erase(e);
            delete tmp;
            e = pos;
            cout<<"explosion deleted";
        }
        e++;
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (game_state == State::GAME){
        switch (key) {
            case 'd':
            case 'D':
            case OF_KEY_RIGHT:  // For movement
                player.right_pressed = true; // in case of multiple inputs
                break;
            case 'a':
            case 'A':
            case OF_KEY_LEFT:
                player.left_pressed = true;
                break;
            case 'w':
            case 'W':
            case OF_KEY_UP:
                player.up_pressed = true;
                break;
            case 's':
            case 'S':
            case OF_KEY_DOWN:
                player.down_pressed = true;
                break;
            case ' ':       // For shooting
                player.space_pressed = true;
                break;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (game_state == State::START){    // starts the game
        if(key == ' '){
            game_state = State::GAME;
        }
    }
    else if (game_state == State::GAME){
        switch (key) {
            case 'd':
            case 'D':
            case OF_KEY_RIGHT:
                player.right_pressed = false;
                break;
            case 'a':
            case 'A':
            case OF_KEY_LEFT:
                player.left_pressed = false;
                break;
            case 'w':
            case 'W':
            case OF_KEY_UP:
                player.up_pressed = false;
                break;
            case 's':
            case 'S':
            case OF_KEY_DOWN:
                player.down_pressed = false;
                break;
            case ' ':
                player.space_pressed = false;
                break;
            case OF_KEY_TAB:
                // pause menu to be implemented later
                game_state= State::PAUSE;
                break;
        }
    }
    else if (game_state==State::PAUSE){
        if (key == ' ')
            game_state= State::GAME;
    }
    else if (game_state== State::GAME_OVER){
        if (key == ' ') {
            setup();
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    if(game_state==State::GAME){
        player.sprite.trans = ofPoint(x,y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

