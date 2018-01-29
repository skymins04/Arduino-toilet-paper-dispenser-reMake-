/*
Name:		Arduino IoT ToiletPaper Dispenser Remake 
Created:	2017-12-29 오후 3:13:57
Author:	10201 강민수(BetaMan)
*/

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED_R 5	//LED_R핀
#define LED_G 4 //LED_G핀
#define MAG 2	//MAG센서 핀
#define LIMIT 1	//LIMIT설정
#define BUZZ 3	//버저 핀

// the setup function runs once when you press reset or power the board

volatile unsigned int count = 0;//, total = 0;	//인터럽트에서 사용하기 위해 volatile 붙임
unsigned long int oldcount = 0, newcount = 0;
bool isOver = false;

unsigned long previousMillis = 0, loopPMillis = 0, unlPMillis = 0;
const short interval = 10, loopInterval = 200, unlockInterval = 500;

void MAGCount() {
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis >= interval) {
		previousMillis = currentMillis;
		count++;
		//total++;
		//Serial.println("Gotcha");
	}
}

void setup() {
	lcd.begin();
	lcd.backlight();
	pinMode(LED_R, OUTPUT);	//LED_R핀 출력
	pinMode(LED_G, OUTPUT); //LED_G핀 출력
	pinMode(MAG, INPUT);	//MAG센서 입력
	attachInterrupt(0, MAGCount, RISING);
	Serial.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() {

	oldcount = count;
	unsigned long currentMillis = millis();

	if (currentMillis - loopPMillis >= loopInterval) {
		loopPMillis = currentMillis;

		newcount = count;
	}

	currentMillis = millis();

	if (currentMillis - unlPMillis >= unlockInterval)
	{
		unlPMillis = currentMillis;
		count = 0;
		isOver = false;
		digitalWrite(LED_R, LOW);
		digitalWrite(LED_G, HIGH);
		lcd.setCursor(0, 0);
		lcd.print("It's OK!!  ");
	}

	if ((oldcount + LIMIT) <= newcount)
	{
		isOver = true;
	}

	if (isOver == true)
	{
		digitalWrite(LED_G, LOW);
		digitalWrite(LED_R, HIGH);
		lcd.setCursor(0, 0);
		lcd.print("It's OVER!!");
		tone(BUZZ, 440, 200);
		delay(100);
	}
	else {
		digitalWrite(LED_R, LOW);
	}
}
