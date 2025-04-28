#ifndef LSYSTEM_H
#define LSYSTEM_H

#include "string_util.h"
#include <map>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include "cube.h"

class LSystem {
public:
	char*** matrix;
	int numCubes;
	std::string fileName;
	std::vector<Cube*> cubes;

	LSystem(std::string fName) {
		fileName = fName;
	}

	void setupMatrix() {
		matrix = (char***)calloc(numCubes, sizeof(char**));
		if (matrix == NULL) {
			std::cerr << "Out of allocatable memory" << std::endl;
			exit(-1);
		}
		for (int i = 0; i < numCubes; i++) {
			matrix[i] = (char**) calloc(numCubes, sizeof(char*));
			if (matrix[i] == NULL) {
				std::cerr << "Out of allocatable memory" << std::endl;
				exit(-1);
			}	
			for (int j = 0; j < numCubes; j++) {
				matrix[i][j] = (char*) calloc(numCubes, sizeof(char));
				if (matrix[i][j] == NULL) {
					std::cerr << "Out of allocatable memory" << std::endl;
					exit(-1);
				}
			}
		}
	}

	std::vector<Cube *> writeCubes() {
		int mid = numCubes / 2;

		for (int i = 0; i < numCubes; i++) {
			for (int j = 0; j < numCubes; j++) {
				for (int k = 0; k < numCubes; k++) {
					if (matrix[i][j][k] == 0) {
						float x = mid - i;
						float y = mid - j;
						float z = mid - k;
						Cube* newCube = new Cube(glm::vec3(x, y, z));
						cubes.push_back(newCube);
					}
				}
			}
		}

		return cubes;
	}

	void clearCubes() {
		for (auto cube : cubes) {
			delete(cube);
		}
	}

	void parse(std::istream& istr) {
		/*
			Format of input files
			---------------------
			numCubes
			numIters
			axiom
			rule1
			rule2
			...
			rulen
			---------------------
			Format of rule
			---------------------
			f : f+g+
			f 0.33 : f+g+
		*/

		unsigned int inIters = 0;
		std::string inAxiom;
		std::multimap<char, std::string> inRules;
		int inCubes;
		std::multimap<char, float> inWeights;
		std::string temp;

		temp = getNextLine(istr);
		inIters = std::stoi(temp);
		temp = getNextLine(istr);
		inCubes = std::stoi(temp);
		inAxiom = getNextLine(istr);

		std::cout << "inIters: " << inIters << "\ninCubes: " << inCubes << "\ninAxiom: " << inAxiom << std::endl;

		while ((temp = getNextLine(istr)) != "EOF") {
			char c = temp[0];
			int pos = temp.find(":") + 2;
			std::string w;
			if (pos > 4) {
				w = temp.substr(2, pos - 3);
			}
			std::string s = temp.substr(pos);
			inRules.insert({ c, s });
			if (pos > 4) {
				inWeights.insert({ c, std::stof(w) });
				std::cout << "Rule: " << c << " -> " << s  << " weight: " << std::stof(w) << std::endl;
			}
			else {
				inWeights.insert({ c, 1.0f });
				std::cout << "Rule: " << c << " -> " << s  << " weight: " << 1.0f << std::endl;
			}
		}
		std::cout << "Successfully parsed" << std::endl;

		rules = std::move(inRules);
		weights = std::move(inWeights);
		numCubes = inCubes;

		setupMatrix();
		strings.push_back(inAxiom);

		for (int i = 0; i < inIters; i++) {
			iterate();
			std::cout << strings.back() << std::endl;
		}

		drawGeometry(strings.back());
	}

	void parseString(std::string str) {
		std::stringstream ss(str);

		ss = preprocessStream(ss);
		parse(ss);
	}

	void parseFile() {
		std::cout << "0" << std::endl;
		std::ifstream file(fileName);
		std::cout << "1" << std::endl;
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open " + fileName);
		}

		std::cout << "2" << std::endl;
		std::stringstream ss = preprocessStream(file);
		std::cout << "Going to parse " << fileName << std::endl;
		parse(ss);
	}

	unsigned int iterate() {
		if (strings.empty()) {
			return 0;
		}

		std::string newString = applyRules(strings.back());

		strings.push_back(newString);
		return strings.size();
	}

	float randWeight(float max) {
		std::uniform_real_distribution<float> urd(0, max);
		return urd(rng);
	}

	std::string searchWeights(const std::vector<std::pair<std::string, float>> cum_weights, const float r) {
		for (auto p : cum_weights) {
			if (r <= p.second) {
				return p.first;
			}
		}
	}

	std::string applyRules(std::string str) {
		std::string ret = "";
		for (char c : str) {
			if (rules.count(c) != 0) {
				auto it = rules.equal_range(c);
				auto wit = weights.equal_range(c);
				auto witem = wit.first;
				std::string add;
				float total = 0.0f;
				std::vector<std::pair<std::string, float>> cum_weights;
				for (auto item = it.first; item != it.second; item++) {
					add = item->second;
					total += witem->second;
					cum_weights.push_back({ item->second, total });
					witem++;
				}
				float r = randWeight(total);
				add = searchWeights(cum_weights, r);
				ret.append(add);
			}
			else {
				ret = ret + c;
			}
		}

		return ret;
	}

	void drawGeometry(std::string str) {
		glm::vec3 start = glm::vec3(numCubes / 2, numCubes / 2, 0);
		glm::vec3 advance = glm::vec3(0, 0, 1);
		glm::vec3 curr = start;
		int max = numCubes;

		for (char c : str) {
			int x = curr.x;
			int y = curr.y;
			int z = curr.z;
			switch (c) {
			case 'f':
				if (x > max || y > max || z > max) {
					break;
				}
				matrix[x][y][z] = 2;
				break;
			}
			curr = curr + advance;
			std::cout << "curr: " << glm::to_string(curr) << std::endl;
		}
	}

private:
	std::multimap<char, std::string> rules;
	std::multimap<char, float> weights;
	std::mt19937 rng;
	std::vector<std::string> strings;
};

#endif LSYSTEM_H