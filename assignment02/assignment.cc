/*===========================================================================*/
/*                                                                           *
 * Copyright (c) 2020, Computer Graphics Group RWTH Aachen University        *
 *                            All rights reserved                            *
 *                                                                           *
 * Basic Techniques in Computer Graphics Exercise                            *
 *                                                                           */
/*===========================================================================*/

#include "assignment.hh"

namespace {
	glm::mat4 scale(const float sx, const float sy) {
		return glm::mat4(
			sx, 0.0, 0.0, 0.0,
			0.0, sy, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}

	glm::mat4 scale(const float s) {
		return scale(s, s);
	}

	glm::mat4 translate(const float x, const float y) {
		return glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			x, y, 0.0, 1.0
		);
	}

	glm::mat4 rotate(const float angle) {
		return glm::mat4(
			cos(angle), sin(angle), 0.0, 0.0,
			-sin(angle), cos(angle), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		);
	}
}

void task::drawScene(int scene, float runTime) { 
	// colors
	const glm::vec3 blue(0.0f, 0.0f, 1.0f);
	const glm::vec3 lightgray(0.4f, 0.4f, 0.4f);
	const glm::vec3 darkgray(0.2f, 0.2f, 0.2f);
	const glm::vec3 black(0.0f, 0.0f, 0.0f);
	const glm::vec3 white(1.0f, 1.0f, 1.0f);
	const glm::vec3 green(0.0f, 1.0f, 0.0f);
	const glm::vec3 yellow(1.0f, 1.0f, 0.0f);

	// pi
	const float pi = 3.141593;


	// (a) track: scaling
	drawCircle(blue, scale(0.9));
	drawCircle(darkgray, scale(0.89));
	drawCircle(blue, scale(0.7));
	drawCircle(black, scale(0.69));

	// (b) stand for the spectators: translation and scaling
	drawCircle(lightgray, translate(-0.95, 0) * scale(0.04, 0.6));
	
	// (c) start / finish line: translation and scaling
	float t = -0.875;
	for (int i = 0; i < 9; i++) {
		drawCircle(white, translate(t, 0) * scale(0.005, 0.03));
		t += 0.02;
	}
	
	// (d) white dotted line: translation, rotation and scaling
	const float r = 0.79;
	for (int i = 0; i < 30; i++) {
		float alpha = (i+0.5) * (12*pi/180);
		drawCircle(white, translate(r*cos(alpha), r * sin(alpha)) * rotate(alpha) * scale(0.01, 0.03));
	}
	
	// (e) two race cars: translation, rotation and scaling

	// draw the cars
	const float r1 = -0.74;
	const float r2 = -0.84;
	float alphay = runTime * 100 * pi / 180;
	float alphag = runTime * 200 * pi / 180;

	// let's goooo
	drawCircle(yellow, translate(r1*cos(alphay), r1*sin(alphay)) * rotate(alphay) * scale(0.02, 0.1));
	drawCircle(green, translate(r2 * cos(alphag), r2 * sin(alphag)) * rotate(alphag) * scale(0.02, 0.1));
	
}


void task::initCustomResources() {}
void task::deleteCustomResources() {}
