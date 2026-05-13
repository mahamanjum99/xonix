                                      //MAHAM ANJUM 24I-0053 & ROMAISA 24I-0061
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<time.h>
#include<fstream>
#include<sstream>
#include<cmath>
#include<iostream>

using namespace sf;
using namespace std;

Font font;
bool loadFont(){
    return font.loadFromFile("ARIAL.TTF");
}

const int M = 25, N = 40;      //No. of rows and columns in grid
int grid[M][N] = { 0 };   // 0->unfilled ares; -1->complete filled tiles ; 1-> boundaries ;2->recently being filled
int ts = 18;
int currentState = 0; // 0: Main Menu, 1: Game, 2: Scoreboard, 3: Levels, 4: Player Mode, 5: End Menu
bool Game = true;
int score = 0;            //current score
int highScore = 0;
int moveCounter = 0;
int selectedLevel = 1;
bool twoPlayerMode = false;
bool isNewHighScore = false;
float powerUpDuration = 0;
bool powerUpActive = false;
int powerUpCount = 0;

struct Enemy{
    int x, y, dx, dy;   //for player
    float speedMultiplier; // ^ -> speed ^
    int movementPattern; // 0: linear, 1: zigzag, 2: circular
    bool isStopped;
    bool active;
    float centerY, centerX;  //for circular movement pattern
   
    Enemy() {
        x = y = 300;
        do { dx = 4 - rand() % 8; } while (dx == 0);
        do { dy = 4 - rand() % 8; } while (dy == 0);
        speedMultiplier = 1.0f;
        movementPattern = 0;
        isStopped = false;
        active=true;
    }
   
    void move(float time) {
        if(isStopped) return;
       
        // Store old position in case of collision
        int oldX = x, oldY = y; // it stores old positions before collisions happens 
       
        switch(movementPattern) {
            case 1: // Zigzag
                static float zigzagTimer = 0;
                zigzagTimer += time;
                x += dx * speedMultiplier;
                y += dy * speedMultiplier * sin(zigzagTimer * 3);
                break;
            case 2: // Circular
                static float angle = 0;
                angle += time * 0.002f;
                if (angle > 2 * 3.14159) angle -= 2 * 3.14159;
                centerX = max(50, min(x - dx * 10, N * ts - 50));
                centerY = max(50, min(y - dy * 10, M * ts - 50));
                x = centerX + cos(angle) * 50;
                y = centerY + sin(angle) * 50;
                break;
            default: // Linear
                x += dx * speedMultiplier;
                y += dy * speedMultiplier;
        }
       
        // Ensure enemies stay within screen bounds
        x = max(0, min(x, N * ts - 1));
        y = max(0, min(y, M * ts - 1));
       
        // Checking collisions with walls
        int gx = x / ts;  
        int gy = y / ts;    

        if(gx < 0) gx = 0;
        if(gy < 0) gy = 0;
        if (gx >= N) gx = N - 1;
        if(gy >= M) gy = M - 1;

        // when enemy hits wall so reverse the direction
        if(grid[gy][gx] == 1) {
            dx = -dx;
            dy = -dy;
            x = oldX;
            y = oldY;
        }

        // we are making sure speed is not 0
        if(dx == 0 && dy == 0) {
            dx = (rand() % 2) ? 1 : -1;
            dy = (rand() % 2) ? 1 : -1;
        }
    }
};

void drop(int y, int x) {
    if (y < 0 || y >= M || x < 0 || x >= N) return;
    if(grid[y][x] != 0) return;

    grid[y][x] = -1;
    drop(y - 1, x);
    drop(y + 1, x);
    drop(y, x - 1);
    drop(y, x + 1);
}

bool allTilesCaptured() {
    for (int i = 1; i < M-1; i++) {
        for (int j = 1; j < N-1; j++) {
            if (grid[i][j] != 1 && grid[i][j] != 2) {
                return false;
            }
        }
    }
    return true;
}

void resetGame(int &enemyCount, Enemy a[], int &x, int &y, int &dx, int &dy,
              float &playTime, float &enemyIncreaseTimer, bool &patternsActivated) {
    Game = true;
    x=10, y=0;
    dx=0, dy=0;
    score=0;
    moveCounter=0;
    playTime=0;
    enemyIncreaseTimer = 0;
    patternsActivated = false;
    isNewHighScore = false;
    powerUpActive = false;
    powerUpCount = 0;
   
    for(int i=1; i<(M-1); i++)
        for(int j=1; j<(N-1); j++)
            grid[i][j]=0;
   
    switch(selectedLevel){
        case 1:
             enemyCount=2;
             break;
        case 2:
             enemyCount=4;
             break;
        case 3:
             enemyCount=6;
             break;
        case 4:
             enemyCount=2;
             break;
    }
   
    // Initialize only the needed enemies
    for(int i=0; i<20; i++) {
        if(i < enemyCount) {
            a[i] = Enemy();  // Active enemy
        } else {
            a[i].active = false;  // inactive
        }
    }
  }

void updateHighScore(int currentScore){
    ifstream inFile("scoreboard.txt");
    int lines=0, temp;
   
    //reading from a file to find no of scores stored
    while(inFile>>temp){
        lines++;
    }
    inFile.close();
   
    int totalScores=lines+1;
    int *scores=new int[totalScores];
   
    inFile.open("scoreboard.txt");
    for(int i=0; i<lines; i++){
        inFile>>scores[i];
    }
    inFile.close();
    scores[lines]=currentScore;
   
    // Sort scores in descending order
    for(int i=0; i<totalScores-1; i++) {
        for(int j=i+1; j<totalScores; j++) {
            if(scores[j]>scores[i]) {
                int temp=scores[i];
                scores[i]=scores[j];
                scores[j]=temp;
            }
        }
    }
   
    ofstream outFile("scoreboard.txt", ios::trunc);
    for(int i=0; i<totalScores; i++) {
        outFile<<scores[i]<<endl;
    }
    outFile.close();
   
    delete[] scores;
}

void loadHighScore() {
    ifstream file("scoreboard.txt");
    if (file.is_open()) {
        file>>highScore;
        file.close();
    }
}

void activatePowerUp(Enemy enemies[], int enemyCount) {
    if(powerUpCount<= 0)
        return;
   
    powerUpCount--;
    powerUpActive=true;
    powerUpDuration=3.0f;
   
    for(int i=0; i<enemyCount; i++){
        enemies[i].isStopped = true;
    }
}

void deactivatePowerUp(Enemy enemies[], int enemyCount) {
    powerUpActive=false;
    for (int i=0; i<enemyCount; i++){
        enemies[i].isStopped = false;
    }
}

int main(){
    srand(time(0));
    if(!loadFont())
        return 1;
    
    // music code
          sf::Music backgroundMusic;
          if(!backgroundMusic.openFromFile("/home/maham/Downloads/xonix/build/bgmusic.ogg")) {
              cerr<<"Error loading background music!"<<endl;
              return -1;
              }
             
             backgroundMusic.setLoop(true);
             backgroundMusic.play();
             bool musicPlaying=true;
             
 
    RenderWindow window(VideoMode(N*ts, M*ts), "Xonix Game!");
    window.setFramerateLimit(60);
   
    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    
    Texture backgroundTexture;
    if(!backgroundTexture.loadFromFile("images/bg.png")) {
    cerr<<"failed to load background image!"<<endl;
    return -1;
    }
    
   
     Sprite backgroundSprite(backgroundTexture);
     backgroundSprite.setScale(
     float(N*ts)/backgroundTexture.getSize().x,
     float(M*ts)/backgroundTexture.getSize().y
     );
     
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");
   
    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    Text text("", font, 20);
    text.setPosition(200, 150);
    text.setFillColor(Color::White);
    text.setStyle(Text::Bold);
    text.setOutlineThickness(3.f);
    text.setOutlineColor(Color::Black);

    Text display;
    display.setFont(font);
    display.setCharacterSize(20);
    display.setFillColor(Color::White);
    display.setPosition(10, 10);
    display.setStyle(Text::Bold);
    display.setOutlineThickness(2.f);
    display.setOutlineColor(Color::Black);

    string mainMenuText, levelsText, playerModeText;
    string endMenuText = "";

    ifstream file;
    file.open("menu.txt");
    if(file.is_open()) {
        string line;
        while(getline(file, line))
            mainMenuText += line + "\n";
        file.close();
    }

    file.open("levels.txt");
    if(file.is_open()) {
        string line;
        while(getline(file, line))
            levelsText += line + "\n";
        file.close();
    }

    file.open("endmenu.txt");
    if(file.is_open()) {
        string line;
        while(getline(file, line))
            endMenuText += line + "\n";
        file.close();
    }
   
    playerModeText = "SELECT PLAYER MODE\n\n1. Single Player\n2. Two Players";
 
   int count=0;
    ifstream scb("scoreboard.txt");
    string line="";
    string scoreboardText = "";
    if(scb.is_open()){
        while(getline(scb, line)) {
        if(count<5){
            scoreboardText+=line + "\n";
            count++;
            }
         else
            continue;
        }
    }

    int x = 10, y = 0, dx = 0, dy = 0;
    float timer = 0, delay = 0.07, playTime = 0;
    float enemyIncreaseTimer = 0, continuousModeTimer = 0;
    bool patternsActivated = false;
    Clock clock;
    int enemyCount = 2;
    Enemy a[20];
   
    for(int i = 0; i < M; i++)
        for(int j = 0; j < N; j++)
            if(i == 0 || j == 0 || i == M-1 || j == N-1)
                grid[i][j] = 1;             //borders

    while(window.isOpen()){
        float time=clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;
        Event e;
 
        while(window.pollEvent(e)){
            if(e.type == Event::Closed)
                window.close();
           
            if(e.type == Event::KeyPressed) {
                // Main menu
                if(currentState==0) {
                    if(e.key.code == Keyboard::Num1)
                        currentState = 3;//levels
                    else if(e.key.code == Keyboard::Num2)
                        currentState = 2;//scoreboard
                    else if(e.key.code == Keyboard::Num3)
                        window.close();//exit
                }
                // Scoreboard
                else if(currentState == 2) {
                    if(e.key.code == Keyboard::Escape)
                        currentState = 0;
                }
                // Levels
                else if(currentState == 3) {
                    if(e.key.code >= Keyboard::Num1 && e.key.code <= Keyboard::Num4) {
                        selectedLevel = e.key.code - Keyboard::Num0;
                        currentState = 4;
                    }
                    else if(e.key.code == Keyboard::Escape)
                        currentState = 0;
                }
                // Player modes
                else if(currentState == 4) {
                    if(e.key.code == Keyboard::Num1 || e.key.code == Keyboard::Num2) {
                        twoPlayerMode = (e.key.code == Keyboard::Num2);
                        resetGame(enemyCount, a, x, y, dx, dy, playTime, enemyIncreaseTimer, patternsActivated);
                        currentState = 1;
                    }
                    else if(e.key.code == Keyboard::Escape)
                        currentState = 3;
                }
                // End menu
                else if(currentState==5) {//end menu
                  if(e.key.code==Keyboard::Num1) 
                  {currentState=3;
                  // restart music when u reach main menu
               if(!musicPlaying){
               backgroundMusic.play();
               musicPlaying=true;
               }}    
                    else if(e.key.code == Keyboard::Num2)
                        currentState = 2;
                    else if(e.key.code == Keyboard::Num3)
                        currentState = 0;
                    else if(e.key.code == Keyboard::Num4)
                        window.close();
                }
                // Game - power up activation
                else if(currentState == 1 && e.key.code == Keyboard::Space) {
                    activatePowerUp(a, enemyCount);
                }
            }
        }
       
        if(currentState == 1 && Game) {
            playTime += time;
            enemyIncreaseTimer+= time;
           
            if(powerUpActive) {
                powerUpDuration -= time;
                if(powerUpDuration <= 0) {
                    deactivatePowerUp(a, enemyCount);
                }
            }
   
            if(selectedLevel == 4) {
                continuousModeTimer += time;
                if(continuousModeTimer >= 20.0f && enemyCount < 20) {
                    a[enemyCount] = Enemy();
                    enemyCount += 2;
                    continuousModeTimer = 0;
                }
            }

            // Player movement
            if(!twoPlayerMode) {
                if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
                if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; }
                if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; }
                if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; }
            }

            if(timer > delay) {
                x += dx;
                y += dy;

                if(x<0)
                    x = 0;
                if(x>N-1) x = N - 1;
                if (y < 0) y = 0;
                if (y > M - 1) y = M - 1;

                if((grid[y][x]==2) || (grid[y][x]==-1))
                     Game = false;
                if(grid[y][x]==0){
                    grid[y][x]=2;
                    score++;
                    moveCounter++;
                   
                    // Bonus points
                    static int tilesCaptured=0;
                    tilesCaptured++;
                    if(tilesCaptured>10){
                        score+=1;
                        tilesCaptured=0;
                    }
                   
                    // Power-up grants
                    if(score==50 || (score > 50 && (score - 50) % 30 == 0)) {
                        powerUpCount++;
                    }
                }
                timer = 0;
            }
                   
            if(enemyIncreaseTimer >= 20.0f) {
                for(int i = 0; i < enemyCount; i++) {
                    a[i].speedMultiplier += 0.2f;
                }
                enemyIncreaseTimer = 0;
            }
           
            if(!patternsActivated && playTime >= 30.0f) {
                int enemiesToChange = max(1, enemyCount / 2);
                for(int i = 0; i < enemiesToChange; i++) {
                    a[i].movementPattern = (i % 2) + 1;
                }
                patternsActivated = true;
            }
           
            for (int i = 0; i < enemyCount; i++) {
                a[i].move(time);
               
                // Check for enemy collisions with player or player areas
                int enemyGridX = a[i].x / ts;
                int enemyGridY = a[i].y / ts;
               
                if ((enemyGridX == x && enemyGridY == y) ||  // Player position
                    grid[enemyGridY][enemyGridX] == 2 ||      // Active trail
                    grid[enemyGridY][enemyGridX] == -1) {     // Filled area
                    Game = false;
                    updateHighScore(score);
                   
                    if(musicPlaying) {
                    backgroundMusic.stop(); // when game ends music should be stopped
                    musicPlaying=false; }
                    
                    stringstream ss;
                    ss << "GAME OVER\nFinal Score: " << score << "\n";
                    if (isNewHighScore) ss << "NEW HIGH SCORE!\n";
                    ss << "Total Moves: " << moveCounter << "\n";
                    ss << "\nOptions:\n1-Restart\n2-Scoreboard\n3-Main Menu\n4-Exit";
                    endMenuText = ss.str();
                    currentState = 5;
                    break;
                }
            }

            if (grid[y][x] == 1) {
                dx = dy = 0;
                for (int i = 0; i < enemyCount; i++)
                    drop(a[i].y / ts, a[i].x / ts);

                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == -1) grid[i][j] = 0;
                        else grid[i][j] = 1;
            }
           
            // Check for victory condition
            if(allTilesCaptured()) {
                Game = false;
                updateHighScore(score);
                   
                                   if(musicPlaying) {
                    backgroundMusic.stop();
                    musicPlaying = false;
                }
                   
                stringstream ss;
                ss << "VICTORY!\n\nYou captured all tiles!\nFinal Score: " << score << "\n";
                if(isNewHighScore) ss << "NEW HIGH SCORE!\n";
                ss << "Time Played: " << static_cast<int>(playTime) << "s\n";
                ss << "Total Moves: " << moveCounter << "\n";
                ss << "\nOptions:\n1-Restart\n2-Scoreboard\n3-Back to main menu\n4-Exit";
                endMenuText = ss.str();
                currentState = 5;
            }
        }
       
        window.clear(currentState == 1 ? Color() : Color::Black);
       
       if(currentState == 0 || currentState == 3 || currentState == 5) {
           window.draw(backgroundSprite);
        }

        if(currentState == 0) {
            text.setString(mainMenuText);
            window.draw(text);
        }
        else if(currentState == 2) {
            text.setString("SCOREBOARD!!\n\n" + scoreboardText);
            window.draw(text);
        }
        else if(currentState == 3) {
            text.setString(levelsText);
            window.draw(text);
        }
        else if(currentState == 4) {
            text.setString(playerModeText);
            window.draw(text);
        }
        else if(currentState == 5) {
            text.setString(endMenuText);
            window.draw(text);
        }
        else if(currentState == 1) {
            // Draw all tiles
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < N; j++) {
                    sTile.setPosition(j * ts, i * ts);
                   
                    if (grid[i][j] == 0)
                        sTile.setColor(Color(112, 128, 144)); // Slate Gray
                    else if (grid[i][j] == 1)
                        sTile.setColor(Color(65, 105, 225)); // Royal Blue
                    else if (grid[i][j] == 2)
                        sTile.setColor(Color(255, 69, 0)); // Orange Red
                    else if (grid[i][j] == -1)
                        sTile.setColor(Color(255, 215, 0)); // Gold
                       
                    window.draw(sTile);
                }
            }

            // Draw player's current position
            sTile.setPosition(x * ts, y * ts);
            sTile.setColor(Color::Green);
            window.draw(sTile);

            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x * ts, y * ts);
            window.draw(sTile);
           
            // Draw enemies
            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; i++) {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }

           // for displaying
            stringstream hudSS;
            hudSS << "Time: " << static_cast<int>(playTime) << "s\n";
            hudSS << "Moves: " << moveCounter << "\n";
            hudSS << "Score: " << score << "\n";
            hudSS << "Power-ups: " << powerUpCount;
            display.setString(hudSS.str());
            window.draw(display);

            if (!Game) window.draw(sGameover);
        }

        window.display();
    }

    return 0;
}
