<?php

// リクエストタイプを表示
echo "[REQUEST_METHOD] " . $_SERVER["REQUEST_METHOD"] . "\n\n";

// GETパラメータがあればダンプ
if(!empty($_GET))
{
	echo "[GET] ---\n";
	var_dump($_GET);
	echo "\n\n";
}

// JSONとしてPOSTされていればダンプ
$json = file_get_contents('php://input');
if(!empty($json))
{
	echo "[POST] ---\n";
	var_dump(json_decode($json, true));
	echo "\n\n";
}
