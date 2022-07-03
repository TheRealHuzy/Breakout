struct BackGroundInfo {
	float position = 0;
	float positionBackGroundY = -0.01;
	float halfSizeBackgroundX = 0.46;
	float halfSizeBackgroundY = 0.47;
	float halfSizeBorder = 0.48;
	int backgroundColor = 0xd8dcff;
	int borderColor = 0xffffff;

	float pointOffset = 0.47;
	float pointMiddle = 0;
	float pointHalfSize = 0.015;
	float pointHalfSizeActive = 0.019;
	int pointColor = 0xffffff;
	int pointColorPassed = 0xaeadf0;
	int pointColorActive = 0xf6ff49;
	int pointColorActiveBorder = 0xaeadf0;

	int level = 1;
	std::string level1 = "Levels\\level.xml";
	std::string level2 = "Levels\\level2.xml";
	std::string level3 = "Levels\\level3.xml";

	float lineOffset1 = 0.24;
	float lineHalfX1 = 0.23;
	float lineOffset2 = 0;
	float lineHalfX2 = 0.47;
	float lineHalfY = 0.008;
	int lineColor = 0xaeadf0;

	float scoreX = 0.0525;
	float scoreXDefault = 0.0525;
	float scoreY = 0;
	float scoreNextX = 0.035;
	float scoreHalfX = 0.01;
	float scoreHalfY = 0.02;
	int scoreColor = 0xaeadf0;
};

struct PlayerInfo {
	float playerX = 0.0;
	float playerY = -0.42;
	float playerXDefault = 0.0;
	float playerYDefault = -0.42;
	float halfX = 0.11;
	float halfY = 0.02;
	float movementSpeed = 0.8;
	float velocity = 0;
	float acceleration = 0;
	float accIncrement = 2;
	float bounce = 0.5;
	float shootAngle = 5;
	int startLifes = 3;
	int currentLifes = 3;
	float lifeOffeset = 0.06;
	float lifeHalf = 0.008;
	int lifeColor = 0xf6ff49;
	int score = 0;
	std::string hitSound = "Sounds\\playerHit.wav";
	std::string loseLifeSound = "Sounds\\loseLife.wav";
	std::string loseGameSound = "Sounds\\loseGame.wav";
	std::string winGameSound = "Sounds\\winGame.wav";
	int color = 0xaeadf0;
};

struct BallInfo {
	float ballX = 0.0;
	float ballY = -0.38;
	float ballXDefault = 0.0;
	float ballYDefault = -0.38;
	float halfBall = 0.01;
	float velocityX = 0;
	float velocityY = 1;
	float velocitymultyplyer = 0.8;
	float multyplyerIncrement = 0.2;
	int color = 0xf6ff49;
};

struct BrickType {
	std::string id = "";
	std::string texture = "";
	int hitPoints = 0;
	std::string hitSound = "";
	std::string breakSound = "";
	int breakScore = 0;
};

const int numberOfBricks = 4;

struct Brick {
	std::string type = "";
	int hitPoints = 0;
	int breakScore = 0;
	int color = 0xffffff;
};

struct LevelInfo {
	int rowCount = 0;
	int columnCount = 0;
	int rowSpacing = 0;
	int columnSpacing = 0;
	BrickType brickTypes[numberOfBricks];
	Brick bricks[200];
	bool beaten = false;
	std::string hitSound = "Sounds\\wallHit.wav";
};

struct BrickPlacementInfo {
	float brickHalfX = 0.02;
	float brickHalfY = 0.01;
	float startX = -0.42;
	float startY = 0.42;
	float startXDefault = -0.42;
	float startYDefault = 0.42;
	float nextX = 0.044;
	float nextY = 0.03;

	float brickHalfXDefault = 0.02;
	float nextYDefaut = 0.03;

	int strongBrickColor = 0x89535d;
	int regularBrickColor = 0xa76571;
	int weakBrickColor = 0xc38d94;
	int impenetrableBrickColor = 0xaaa9ad;
};