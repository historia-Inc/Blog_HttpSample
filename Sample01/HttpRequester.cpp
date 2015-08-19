#include "HttpTest.h"
#include "HttpRequester.h"

AHttpRequester::AHttpRequester(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void AHttpRequester::AddRequestParameter(const FString& Key, const FString& Value)
{
	RequestParameters.Add(Key, Value);
}

void AHttpRequester::RemoveRequestParameter(const FString& Key)
{
	RequestParameters.Remove(Key);
}

void AHttpRequester::StartRequest()
{
	auto& Module = FHttpModule::Get();
	auto& Manager = Module.GetHttpManager();

	// リクエストを作成
	TSharedRef<IHttpRequest> Request = Module.CreateRequest();

	// リクエストタイプを設定
	Request->SetVerb(RequestType);

	// URL、Content設定
	if(RequestType == "GET")
	{
		// GET通信の場合、URLにリクエストパラメータを付与
		auto UrlWithParameter = Url;
		int ParameterIndex = 0;
		for (const auto& Entry : RequestParameters)
		{
			UrlWithParameter += (ParameterIndex++ == 0) ? TEXT("?") : TEXT("&");
			UrlWithParameter += Entry.Key;
			UrlWithParameter += TEXT("=");
			UrlWithParameter += Entry.Value;
		}

		Request->SetURL(UrlWithParameter);
	}
	else
	{
		// それ以外の場合、JSON文字列としてリクエストパラメータを付与
		TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		for (const auto& Entry : RequestParameters)
		{
			JsonObject->SetStringField(*Entry.Key, *Entry.Value);
		}

		// Content文字列にJSONフォーマットで書き出し
		FString Content;
		auto Writer = TJsonWriterFactory<>::Create(&Content);
		FJsonSerializer::Serialize(JsonObject, Writer);

		Request->SetURL(Url);
		Request->SetContentAsString(Content);

		// Content-TypeをJSONとして設定
		Request->SetHeader("Content-Type", "application/json");
	}

	// 完了時イベントをバインド
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpRequester::OnComplete);

	// 通信開始
	Request->ProcessRequest();

	// マネージャーの管理対象に追加
	Manager.AddRequest(Request);
}

void AHttpRequester::OnComplete(FHttpRequestPtr HttpRequestPtr, FHttpResponsePtr HttpResponsePtr, bool bSuccessed)
{
	auto& Module = FHttpModule::Get();
	auto& Manager = Module.GetHttpManager();

	// コンソールにレスポンスの内容を出力
	FString ResponseText = bSuccessed ? HttpResponsePtr->GetContentAsString() : TEXT("Request failed.");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *ResponseText);

	// マネージャーの管理対象から削除
	Manager.RemoveRequest(HttpRequestPtr.ToSharedRef());
}
