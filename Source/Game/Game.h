#pragma once

#include "Engine.h"

class Game final : public Engine
{
public:
	Game() = default;
	~Game() = default;

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	virtual void Init() override;
	virtual void LoadContent() override;
	virtual void UpdateGame(float time) override;
	virtual void UnloadContent() override;
	virtual void DeInit() override;

private:

	// Scene setup
	void CompressionTestScene();
	void MipmappingDemo();
	void TintedMipDemo();
	void ModelTestScene();
	void AnimationTestScene();

	void ProcessPackages();
	void PreparePipelines();
	void CompileShaders();
};