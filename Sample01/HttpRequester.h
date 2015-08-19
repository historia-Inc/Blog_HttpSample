#pragma once

#include "GameFramework/Actor.h"
#include "HttpRequester.generated.h"

UCLASS()
class HTTPTEST_API AHttpRequester : public AActor
{
	GENERATED_UCLASS_BODY()

private:
	// リクエストパラメータ
	TMap<FString, FString> RequestParameters;

public:
	// 接続先URL
	UPROPERTY(BlueprintReadWrite, Category="Http")
	FString Url;

	// リクエストタイプ（GET, PUT, POST）
	UPROPERTY(BlueprintReadWrite, Category="Http")
	FString RequestType;

public:
	// リクエストパラメータ追加
	UFUNCTION(BlueprintCallable, Category="Http")
	void AddRequestParameter(const FString& Key, const FString& Value);

	// リクエストパラメータ削除
	UFUNCTION(BlueprintCallable, Category="Http")
	void RemoveRequestParameter(const FString& Key);

	// HTTP通信リクエスト開始
	UFUNCTION(BlueprintCallable, Category="Http")
	void StartRequest();

private:
	// HTTP通信完了時コールバック
	void OnComplete(FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSuccessed);
};
