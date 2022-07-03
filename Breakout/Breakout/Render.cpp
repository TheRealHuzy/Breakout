int clampValue(int value, int maxValue) {
	
	if (value < 0) {
		return 0;
	}
	else if (value > maxValue) {
		return maxValue;
	}
	else {
		return value;
	}
}

void rClearScreen() {
	
	unsigned int* pixel = (unsigned int*)si.bufferMemory;
	for (int i = 0; i < si.screenWidth; i++) {
		for (int j = 0; j < si.screenHeight; j++) {
			*pixel++ = si.clsBackgroundColor;
		}
	}
}

void rRegularShape(float x, float halfX, float y, float halfY, unsigned int color) {
	
	int multiplyValue;

	if (si.screenWidth > si.screenHeight) {
		multiplyValue = si.screenHeight;
	}
	else {
		multiplyValue = si.screenWidth;
	}

	halfX = halfX * multiplyValue;
	halfY = halfY * multiplyValue;
	
	x = x * multiplyValue;
	y = y * multiplyValue;

	x = x + si.screenWidth / 2.0;
	y = y + si.screenHeight / 2.0;

	int xS = x - halfX;
	int xE = x + halfX;
	int yS = y - halfY;
	int yE = y + halfY;

	xS = clampValue(xS, si.screenWidth);
	xE = clampValue(xE, si.screenWidth);
	yS = clampValue(yS, si.screenHeight);
	yE = clampValue(yE, si.screenHeight);

	for (int i = yS; i < yE; i++) {
		unsigned int* pixel = (unsigned int*)si.bufferMemory + xS + i * si.screenWidth;
		for (int j = xS; j < xE; j++) {
			*pixel++ = color;
		}
	}
}