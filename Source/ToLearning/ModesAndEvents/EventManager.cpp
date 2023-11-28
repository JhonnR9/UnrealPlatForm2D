// Create by Davidson


#include "EventManager.h"


FOnTileMapSpawn* UEventManager::GetOnTileMapSpawn() {
	static FOnTileMapSpawn OnTileMapSpawn;
	return &OnTileMapSpawn;
}

FOnChangePlayerPosition* UEventManager::GetOnChangePlayerPosition() {
	static FOnChangePlayerPosition OnChangePlayerPosition;
	return &OnChangePlayerPosition;
}
