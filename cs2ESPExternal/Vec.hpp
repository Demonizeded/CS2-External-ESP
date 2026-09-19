#pragma once
#include <vector>

namespace math {
	struct Vec4 {
		float x, y, z, w;
	};

	struct Vec3 {
		float x, y, z;

		Vec3 operator+(const Vec3& other) const {
			return {x + other.x, y + other.y, z + other.z};
		}
	};

	struct Vec2 {
		float x, y;

		Vec2 operator+(const Vec2 other) const {
			return { x + other.x, y + other.y };
		}

		Vec2 operator*(float sumScr) const{
			return { x * sumScr, y * sumScr};
		}
	};

	struct Player {
		Vec3 pos;
		int health;
		int teamNum;
		std::vector<Vec3> bonePos;
	};

	struct BoneData {
		Vec3 pos;
		float scale;
		Vec4 rot;
	};

	struct BoneConnection {
		int bone1;
		int bone2;
	};

	const std::vector<math::BoneConnection> boneConnections = {
		//Head and Spine
		{ 7, 6 },   // Head - Neck
		{ 6, 23 },  //Neck - Spine2
		{ 23, 3 },  //Spine2 - Spine1
		{ 3, 1 },   //Spine1 - Pelvis

		//Left Hand
		{ 23, 9 },   // Spine2 -> leftShoulder
		{ 9, 10 },   // leftShoulder - leftElbow
		{ 10, 11 },  // leftElbow - leftPalm

		//Right Hand
		{ 23, 13 },  // Spine2 -> rightShoulder
		{ 13, 14 },  // rightShoulder - right Elbow
		{ 14, 15 },  // rightElbow - rightPalm

		//Left Leg
		{ 1, 17 },  // Pelvis - leftThigh
		{ 17, 18 }, // leftThigh - leftKnee
		{ 18, 19 }, // leftKnee - left Feet

		//right Leg
		{ 1, 20 },  // peliv - right thigh
		{ 20, 21 }, // rightThigh - right knee
		{ 21, 22 }  // rightKnee - rightFeet
	};

	struct gameData {
		int teamNum;
		float vm[4][4];
		std::vector<math::Player> players;
	};

};