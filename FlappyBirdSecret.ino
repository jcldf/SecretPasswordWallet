#include <Arduboy2.h>
#include <Keyboard.h>

Arduboy2 arduboy;

#define MAX_ENTRIES 20

// Estrutura para armazenar uma entrada de senha
struct PasswordEntry {
  char site[16];
  char password[16];
};

PasswordEntry entries[MAX_ENTRIES];
uint8_t currentEntry = 0;

const uint8_t unlockSequence[] = {UP_BUTTON, UP_BUTTON, DOWN_BUTTON, LEFT_BUTTON, RIGHT_BUTTON};
uint8_t unlockIndex = 0;
bool unlocked = false;

// Variáveis para o Flappy Bird rudimentar
int birdY = 32;
int birdVelocity = 0;
int obstacleX = 128;
int obstacleHoleY = random(20, 44);
const int gravity = 1;
const int jumpStrength = -3;
const int obstacleWidth = 10;
const int obstacleSpeed = 2;

void drawFlappyBirdGame() {
  // Atualizar posição do pássaro
  birdVelocity += gravity;
  birdY += birdVelocity;
  
  // Desenhar pássaro
  arduboy.fillRect(20, birdY, 5, 5, WHITE);
  
  // Atualizar e desenhar obstáculo
  obstacleX -= obstacleSpeed;
  if (obstacleX + obstacleWidth < 0) {
    obstacleX = 128;
    obstacleHoleY = random(20, 44);
  }
  arduboy.fillRect(obstacleX, 0, obstacleWidth, obstacleHoleY - 5, WHITE);
  arduboy.fillRect(obstacleX, obstacleHoleY + 5, obstacleWidth, 64 - obstacleHoleY - 5, WHITE);
  
  // Checar colisões
  if (birdY < 0 || birdY > 59 || (birdY < obstacleHoleY - 5 || birdY + 5 > obstacleHoleY + 5) && birdY < 64 && obstacleX < 25) {
    birdY = 32;
    birdVelocity = 0;
    obstacleX = 128;
    obstacleHoleY = random(20, 44);
  }
  
  // Pular
  if (arduboy.justPressed(A_BUTTON)) {
    birdVelocity = jumpStrength;
  }
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  
  strcpy(entries[0].site, "Google");
  strcpy(entries[0].password, "1245345asdfqwe"); //put your passwords here
  
  strcpy(entries[1].site, "Instagram");
  strcpy(entries[1].password, "password1232222");
  
  strcpy(entries[2].site, "Outlook");
  strcpy(entries[2].password, "outlookemail123");

  strcpy(entries[3].site, "bank");
  strcpy(entries[3].password, "p08000282234sdf");
}

void drawEntry(uint8_t index) {
  arduboy.drawRoundRect(0, 0, 128, 16, 3, WHITE);  // Contorno superior
  arduboy.drawRoundRect(0, 16, 128, 16, 3, WHITE); // Contorno inferior
  
  arduboy.setCursor(5, 2);
  arduboy.print("Site: ");
  arduboy.print(entries[index].site);
  
  arduboy.setCursor(5, 18);
  arduboy.print("Pass: ");
  
  for (uint8_t i = 0; i < 3 && i < strlen(entries[index].password); i++) {
    arduboy.print(entries[index].password[i]);
  }
  
  for (uint8_t i = 3; i < strlen(entries[index].password); i++) {
    arduboy.print('*');
  }
}

void typePassword(const char* password) {
  Keyboard.begin();
  delay(500);
  for (uint8_t i = 0; i < strlen(password); i++) {
    Keyboard.write(password[i]);
    delay(50);
  }
  Keyboard.end();
}

void loop() {
  if (!arduboy.nextFrame()) {
    return;
  }

  arduboy.pollButtons();
  arduboy.clear();

  if (!unlocked) {
    if (arduboy.justPressed(unlockSequence[unlockIndex])) {
      unlockIndex++;
      if (unlockIndex == sizeof(unlockSequence)) {
        unlocked = true;
      }
    } else if (arduboy.pressed(UP_BUTTON | DOWN_BUTTON | LEFT_BUTTON | RIGHT_BUTTON | A_BUTTON | B_BUTTON)) {
      unlockIndex = 0;
    }
    drawFlappyBirdGame();  // Desenha o Flappy Bird rudimentar
  } else {
    if (arduboy.justPressed(UP_BUTTON) && currentEntry > 0) {
      currentEntry--;
    }

    if (arduboy.justPressed(DOWN_BUTTON) && currentEntry < MAX_ENTRIES - 1) {
      currentEntry++;
    }

    if (arduboy.justPressed(A_BUTTON)) {
      typePassword(entries[currentEntry].password);
    }

    drawEntry(currentEntry);
  }
  
  arduboy.display();
}
