#pragma once
#ifndef _SCENE_HPP
#define _SCENE_HPP

namespace lur {

	class Scene {
	public:
		Scene();
		~Scene();

		void tick(unsigned int dTime);
		void draw();
		void drawGui();

	};

}

#endif
