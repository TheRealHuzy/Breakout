BackGroundInfo bgi;
PlayerInfo pi;
BallInfo bali;
BrickPlacementInfo bpi;

#include "Sound.cpp"

float invert(float value) {
	return value *= -1;
}

void resetBallNPlayer() {
	bali.ballX = bali.ballXDefault;
	bali.ballY = bali.ballYDefault;
	pi.playerX = pi.playerXDefault;
	pi.playerY = pi.playerYDefault;
	pi.velocity = 0;
}

void giveBackLifeIncreaseDiff() {
	if (pi.currentLifes < pi.startLifes) {
		pi.currentLifes++;
	}
	bali.velocitymultyplyer += bali.multyplyerIncrement;
}

void drawBgNProgress() {
	rRegularShape(bgi.position, bgi.halfSizeBorder, bgi.position, bgi.halfSizeBorder, bgi.borderColor);
	rRegularShape(bgi.position, bgi.halfSizeBackgroundX, bgi.positionBackGroundY, bgi.halfSizeBackgroundY, bgi.backgroundColor);

	if (bgi.level == 1) {
		rRegularShape(-bgi.pointOffset, bgi.pointHalfSizeActive, bgi.pointOffset, bgi.pointHalfSizeActive, bgi.pointColorActiveBorder);
		rRegularShape(-bgi.pointOffset, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColorActive);
		rRegularShape(bgi.pointMiddle, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColor);
		rRegularShape(bgi.pointOffset, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColor);
	}
	else if (bgi.level == 2) {
		rRegularShape(-bgi.lineOffset1, bgi.lineHalfX1, bgi.pointOffset, bgi.lineHalfY, bgi.lineColor);

		rRegularShape(-bgi.pointOffset, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColorPassed);
		rRegularShape(bgi.pointMiddle, bgi.pointHalfSizeActive, bgi.pointOffset, bgi.pointHalfSizeActive, bgi.pointColorActiveBorder);
		rRegularShape(bgi.pointMiddle, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColorActive);
		rRegularShape(bgi.pointOffset, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColor);
	}
	else if (bgi.level == 3) {
		rRegularShape(-bgi.lineOffset2, bgi.lineHalfX2, bgi.pointOffset, bgi.lineHalfY, bgi.lineColor);

		rRegularShape(-bgi.pointOffset, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColorPassed);
		rRegularShape(bgi.pointMiddle, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColorPassed);
		rRegularShape(bgi.pointOffset, bgi.pointHalfSizeActive, bgi.pointOffset, bgi.pointHalfSizeActive, bgi.pointColorActiveBorder);
		rRegularShape(bgi.pointOffset, bgi.pointHalfSize, bgi.pointOffset, bgi.pointHalfSize, bgi.pointColorActive);
	}
}

void handleInput(Input* input, float frameTime) {

	pi.acceleration = 0;

	if (input->buttons[btnLeft].pressed) {
		pi.acceleration = pi.acceleration - pi.accIncrement;
	}
	else if (input->buttons[btnRight].pressed) {
		pi.acceleration = pi.acceleration + pi.accIncrement;
	}

	pi.acceleration = pi.acceleration - pi.velocity * 3;

	pi.playerX = pi.playerX + pi.velocity * frameTime + pi.acceleration * frameTime * frameTime / 2;
	pi.velocity = pi.velocity + pi.acceleration * frameTime;
}

void playerWallCollision(LevelInfo* li) {
	
	if (pi.playerX + pi.halfX > bgi.halfSizeBackgroundX) {
		pi.playerX = bgi.halfSizeBackgroundX - pi.halfX;
		pi.velocity = invert(pi.velocity) * pi.bounce;
		playSound(li->hitSound);
	}
	if (-bgi.halfSizeBackgroundX > pi.playerX - pi.halfX) {
		pi.playerX = pi.halfX - bgi.halfSizeBackgroundX;
		pi.velocity = invert(pi.velocity) * pi.bounce;
		playSound(li->hitSound);
	}
}

void ballWallCollision(LevelInfo* li) {
	
	if (bali.ballX + bali.halfBall > bgi.halfSizeBackgroundX) {
		bali.ballX = bgi.halfSizeBackgroundX - bali.halfBall;
		bali.velocityX = invert(bali.velocityX);
		playSound(li->hitSound);
	}
	if (-bgi.halfSizeBackgroundX > bali.ballX - bali.halfBall) {
		bali.ballX = bali.halfBall - bgi.halfSizeBackgroundX;
		bali.velocityX = invert(bali.velocityX);
		playSound(li->hitSound);
	}
	if (bali.ballY + bali.halfBall > bgi.halfSizeBackgroundY) {
		bali.ballY = bgi.halfSizeBackgroundY - bali.halfBall;
		bali.velocityY = invert(bali.velocityY);
		playSound(li->hitSound);
	}
}

void playerBallCollision() {
	
	if (bali.ballY + bali.halfBall < pi.playerY + pi.halfY &&
		bali.ballY + bali.halfBall > pi.playerY - pi.halfY &&
		bali.ballX - bali.halfBall < pi.playerX + pi.halfY * 5.5 &&
		bali.ballX + bali.halfBall > pi.playerX - pi.halfY * 5.5
		) {
			bali.ballY = pi.playerY + pi.halfY + bali.halfBall;
			bali.velocityY = invert(bali.velocityY);
			bali.velocityX = (bali.ballX - pi.playerX) * pi.shootAngle;
			playSound(pi.hitSound);
	}
}

void ballBrickCollision(LevelInfo* li) {
	
	for (int i = 0; i < li->rowCount; i++) {
		for (int j = 0; j < li->columnCount; j++) {
			int k = i * li->columnCount + j;
			if (li->bricks[k].hitPoints > 0) {
				if (bali.ballY + bali.halfBall < bpi.startY + bpi.brickHalfY &&
					bali.ballY + bali.halfBall > bpi.startY - bpi.brickHalfY &&
					bali.ballX - bali.halfBall < bpi.startX + bpi.brickHalfX &&
					bali.ballX + bali.halfBall > bpi.startX - bpi.brickHalfX
					) {
					bali.velocityY = invert(bali.velocityY);
					if (li->bricks[k].type.compare("I") != 0) {
						li->bricks[k].hitPoints--;
					}
					if (li->bricks[k].hitPoints > 0) {
						playSound(li->brickTypes->hitSound);
					}
					else {
						playSound(li->brickTypes->breakSound);
						pi.score += li->bricks[k].breakScore;

						li->beaten = true;
						for (Brick& b : li->bricks) {
							if (b.hitPoints > 0 && b.hitPoints < 10) {
								li->beaten = false;
								break;
							}
						}
					}
				}
			}
			bpi.startX += bpi.nextX;
		}
		bpi.startY -= bpi.nextY;
		bpi.startX = bpi.startXDefault;
	}
	bpi.startX = bpi.startXDefault;
	bpi.startY = bpi.startYDefault;
}

void ballOffScreen() {
	
	if (bali.ballY - bali.halfBall < -bgi.halfSizeBorder - 1) {
		bali.velocityY = invert(bali.velocityY);
		bali.velocityX = 0;

		resetBallNPlayer();
		pi.currentLifes--;
		playSound(pi.loseLifeSound);
	}
}

void drawPlayerNBall() {
	
	rRegularShape(pi.playerX, pi.halfX, pi.playerY, pi.halfY, pi.color);
	if (pi.startLifes == pi.currentLifes) {
		rRegularShape(pi.playerX + pi.lifeOffeset, pi.lifeHalf, pi.playerY, pi.lifeHalf, pi.lifeColor);
	}
	if (pi.startLifes - 1 <= pi.currentLifes) {
		rRegularShape(pi.playerX - pi.lifeOffeset, pi.lifeHalf, pi.playerY, pi.lifeHalf, pi.lifeColor);
	}
	if (pi.startLifes - 2 <= pi.currentLifes) {
		rRegularShape(pi.playerX, pi.lifeHalf, pi.playerY, pi.lifeHalf, pi.lifeColor);
	}

	rRegularShape(bali.ballX, bali.halfBall, bali.ballY, bali.halfBall, bali.color);
}

void drawBricks(LevelInfo* li) {
	
	for (int i = 0; i < li->rowCount; i++) {
		for (int j = 0; j < li->columnCount; j++) {
			int k = i * li->columnCount + j;
			if (li->bricks[k].hitPoints > 0) {

				if (li->bricks[k].hitPoints == 3) {
					rRegularShape(bpi.startX, bpi.brickHalfX, bpi.startY, bpi.brickHalfY, bpi.strongBrickColor);
				}
				else if (li->bricks[k].hitPoints == 2) {
					rRegularShape(bpi.startX, bpi.brickHalfX, bpi.startY, bpi.brickHalfY, bpi.regularBrickColor);
				}
				else if (li->bricks[k].hitPoints == 1) {
					rRegularShape(bpi.startX, bpi.brickHalfX, bpi.startY, bpi.brickHalfY, bpi.weakBrickColor);
				}
				else if (li->bricks[k].hitPoints == 10) {
					rRegularShape(bpi.startX, bpi.brickHalfX, bpi.startY, bpi.brickHalfY, bpi.impenetrableBrickColor);
				}
			}
			bpi.startX += bpi.nextX;
		}
		bpi.startY -= bpi.nextY;
		bpi.startX = bpi.startXDefault;
	}
	bpi.startX = bpi.startXDefault;
	bpi.startY = bpi.startYDefault;
}

void drawFullRight() {
	rRegularShape(bgi.scoreX + bgi.scoreHalfX, bgi.scoreHalfX / 2.1, bgi.scoreY, bgi.scoreHalfY, bgi.scoreColor);
}
void drawFullLeft() {
	rRegularShape(bgi.scoreX - bgi.scoreHalfX, bgi.scoreHalfX / 2.1, bgi.scoreY, bgi.scoreHalfY, bgi.scoreColor);
}
void drawFullMiddleV() {
	rRegularShape(bgi.scoreX, bgi.scoreHalfX, bgi.scoreY, bgi.scoreHalfY, bgi.scoreColor);
}
void drawFullTop() {
	rRegularShape(bgi.scoreX, bgi.scoreHalfX * 1.5, bgi.scoreY + bgi.scoreHalfY / 1.4, bgi.scoreHalfY / 3.5, bgi.scoreColor);
}
void drawFullMiddleH() {
	rRegularShape(bgi.scoreX, bgi.scoreHalfX * 1.5, bgi.scoreY, bgi.scoreHalfY / 3.5, bgi.scoreColor);
}
void drawFullBot() {
	rRegularShape(bgi.scoreX, bgi.scoreHalfX * 1.5, bgi.scoreY - bgi.scoreHalfY / 1.4, bgi.scoreHalfY / 3.5, bgi.scoreColor);
}
void drawHalfTopRight() {
	rRegularShape(bgi.scoreX + bgi.scoreHalfX, bgi.scoreHalfX / 2.1, bgi.scoreY + bgi.scoreHalfY / 2, bgi.scoreHalfY / 2, bgi.scoreColor);
}
void drawHalfTopLeft() {
	rRegularShape(bgi.scoreX - bgi.scoreHalfX, bgi.scoreHalfX / 2.1, bgi.scoreY + bgi.scoreHalfY / 2, bgi.scoreHalfY / 2, bgi.scoreColor);
}
void drawHalfBotRight() {
	rRegularShape(bgi.scoreX + bgi.scoreHalfX, bgi.scoreHalfX / 2.1, bgi.scoreY - bgi.scoreHalfY / 2, bgi.scoreHalfY / 2, bgi.scoreColor);
}
void drawHalfBotLeft() {
	rRegularShape(bgi.scoreX - bgi.scoreHalfX, bgi.scoreHalfX / 2.1, bgi.scoreY - bgi.scoreHalfY / 2, bgi.scoreHalfY / 2, bgi.scoreColor);
}
void drawAllHLines() {
	drawFullTop();
	drawFullMiddleH();
	drawFullBot();
}

void drawScore() {
	
	int score = pi.score;
	while (score) {
		int digit = score % 10;
		score = score / 10;

		switch (digit) {

			case 0: {
				drawFullRight();
				drawFullLeft();
				drawFullTop();
				drawFullBot();
			} break;
			case 1: {
				drawFullMiddleV();
			} break;
			case 2: {
				drawAllHLines();
				drawHalfTopRight();
				drawHalfBotLeft();
			} break;
			case 3: {
				drawAllHLines();
				drawFullRight();
			} break;
			case 4: {
				drawHalfTopLeft();
				drawFullMiddleH();
				drawFullRight();
			} break;
			case 5: {
				drawAllHLines();
				drawHalfTopLeft();
				drawHalfBotRight();
			} break;
			case 6: {
				drawAllHLines();
				drawFullLeft();
				drawHalfBotRight();
			} break;
			case 7: {
				drawFullTop();
				drawFullRight();
			} break;
			case 8: {
				drawAllHLines();
				drawFullLeft();
				drawFullRight();
			} break;
			case 9: {
				drawAllHLines();
				drawHalfTopLeft();
				drawFullRight();
			}
		}
		bgi.scoreX -= bgi.scoreNextX;
	}
	bgi.scoreX = bgi.scoreXDefault;
}

void simulate(Input* input, float frameTime, LevelInfo* li) {
	
	handleInput(input, frameTime);

	bali.ballX += bali.velocityX * frameTime * bali.velocitymultyplyer;
	bali.ballY += bali.velocityY * frameTime * bali.velocitymultyplyer;

	playerWallCollision(li);
	ballWallCollision(li);
	playerBallCollision();
	ballBrickCollision(li);

	ballOffScreen();

	drawBgNProgress();
	drawScore();
	drawPlayerNBall();
	drawBricks(li);
}