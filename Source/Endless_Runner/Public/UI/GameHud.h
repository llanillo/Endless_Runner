// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h" 
#include "GameMode/EndlessRunnerGameMode.h"
#include "GameHud.generated.h"

class UTextBlock;

UCLASS()
class ENDLESS_RUNNER_API UGameHud : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION()
	void OnPauseClick();
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CoinCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LivesCount;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PauseBtn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	
	UFUNCTION(BlueprintCallable)
	void InitializeHud(AEndlessRunnerGameMode* MainGameMode);

	UFUNCTION(BlueprintCallable)
	void SetCoinCount(int32 Count);

	UFUNCTION(BlueprintCallable)
	void SetLivesCount(int32 Count);

protected:

	virtual void NativeConstruct() override;
	
};
