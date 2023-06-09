#include "SceneManager.h"
#include "Secne.h"

void
SceneManager::ChangeScene(Scene* scene)
{
	//今ある要素をすべて削除する
	for (int i = static_cast<int>(m_scenes.size() - 1); i >= 0; i--)
	{
		delete m_scenes.front();
		m_scenes.pop_front();
	}

	m_scenes.push_front(scene);//一個増える
}

void SceneManager::PushScene(Scene* scene)
{
	m_scenes.push_front(scene);//一個増える
}

void SceneManager::PopScene()
{
	//ポップの結果シーンが0にならないようにする
	if (m_scenes.size() > 1)
	{
		//◇次のシーンに移動するためにtopにあるSceneのポインタを消す
		//◇ポインタを消すだけだとscenes_には残っているからpopで一番上の要素を消している
		delete m_scenes.front();//topはてっぺんの要素を返している
		m_scenes.pop_front();//一個減る→普通は0になる
	}
}

void
SceneManager::Update(Mouse& mouse)
{
	m_scenes.front()->Update(mouse);
}

void
SceneManager::Draw()
{
	for (int i = static_cast<int>(m_scenes.size() - 1); i >= 0; i--)
	{
		m_scenes[i]->Draw();
	}
}

void SceneManager::End()
{
	//要素をすべて消す
	for (int i = static_cast<int>(m_scenes.size() - 1); i >= 0; i--)
	{
		delete m_scenes.front();
		m_scenes.pop_front();
	}
}
