/**
 * @file   AnalysisDrive.h
 * @brief  Analysis Drive Include Header File
 * @author kyo
 * @date   2015/05/01
 */
#ifndef ANALYSISDRIVE_ANALYSISDRIVE_H_
#define ANALYSISDRIVE_ANALYSISDRIVE_H_

//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------
/* コマンドライン処理 */
#include "CommandLineSetting.h" // コマンドライン読み込み設定
#include "CommandLineParser.h"  // コマンドライン読み込み機能
#include "Command.h"            // コマンドラインに入力された引数の情報
/* JSON処理 */
#include "Json.h"               // JSONオブジェクト
#include "JsonParser.h"         // JSONパーサー
/* その他 */
#include "EasyTimer.h"          // 時間計測

//------------------------------------------------------------------------------
// Link Libraries
//------------------------------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "AnalysisDrive_Debug.lib") // Debug版
#else
#pragma comment(lib, "AnalysisDrive.lib") // Release版
#endif

#endif // ANALYSISDRIVE_ANALYSISDRIVE_H_
