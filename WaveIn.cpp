#include "stdafx.h"
#include "Syaku8Tuner.h"
#include "Syaku8TunerDoc.h"
#include "Syaku8TunerView.h"
#include "math.h"
#include <mmsystem.h>
#include "s8header.h"
HGLOBAL			hGMemRecBuf;
HWAVEIN			hWave			= NULL;
BUFTYPE			*wavebuf		= NULL;
LPWAVEHDR		wh[MAX_BUFFERS];
//bool			bWaveOpened		= false;
//void CloseWaveInputDevice(void);

/////////////////////////////////////////////////////////////////////////////
// CS8tunerView クラスの描画

//*******************************************************************
// 録音デバイスを開く - 成功したらTRUEを返す
//*******************************************************************

bool OpenWaveInputDevice(
	HWND hWnd)
{
	WAVEFORMATEX wfx;
	MMRESULT rc;
	char err[200];
	wfx.wFormatTag		= WAVE_FORMAT_PCM;		// PCM without any compression
	wfx.nChannels		= 1;					// 1channel (Monaural)
	wfx.nSamplesPerSec	= NYQUIST * 2;			// (Hz)  22050  NYQUIST			11025
	wfx.wBitsPerSample	= sizeof(BUFTYPE) * 8;	// (bits/channel) 16  BUFSZ			8192
	wfx.nBlockAlign		= (WORD)(wfx.nChannels * wfx.wBitsPerSample / 8);
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize			= 0;

	hWave = NULL;
//	return ( waveInOpen(&hWave, WAVE_MAPPER, &wfx, (DWORD)(void*)WaveInProc, (DWORD)hWnd, CALLBACK_FUNCTION)
//			 == MMSYSERR_NOERROR );
	rc=waveInOpen(&hWave, WAVE_MAPPER, &wfx, (DWORD)hWnd,NULL,CALLBACK_WINDOW);
	if (rc != MMSYSERR_NOERROR){
		waveInGetErrorText(rc, err, 128);
		MessageBox(NULL, err,"waveInOpenErr", MB_OK);
		return false;
	}
	return true;
}

//*******************************************************************
// 録音開始
//*******************************************************************
//HGLOBAL			hGMemRecBuf;
void StartRecording(void)
{
	MMRESULT rc;
	int		 i;
	LPSTR	 h;

	//*******************************************************************
	// 録音バッファを確保する
	//*******************************************************************
	if( ( hGMemRecBuf = GlobalAlloc( GPTR, ( sizeof(WAVEHDR) + BUFSZ/BUFDIV*sizeof(BUFTYPE) ) * MAX_BUFFERS ) )	== NULL ) {
		waveInClose(hWave);
		bWaveOpened = false;
		return;
	}

	h = (LPSTR) hGMemRecBuf;
	for (i = 0; i < MAX_BUFFERS; i++) {
	   wh[i] = (LPWAVEHDR) h;
	   h += sizeof(WAVEHDR);
	   wh[i]->lpData = h;
	   h += ( wh[i]->dwBufferLength = BUFSZ / BUFDIV * sizeof(BUFTYPE) );
	}

	// バッファブロックを準備して、入力キューに追加する
	for (i=0; i<MAX_BUFFERS; i++) {
		rc = waveInPrepareHeader(hWave, wh[i], sizeof(WAVEHDR));

		// 入力キューにバッファを追加する
		if (rc == MMSYSERR_NOERROR)
			rc = waveInAddBuffer(hWave, wh[i], sizeof(WAVEHDR));

	}

	// 録音を開始する
	if (rc == MMSYSERR_NOERROR)
		rc = waveInStart(hWave);

	if (rc != MMSYSERR_NOERROR) {
		CloseWaveInputDevice();	 // 割り当てられたメモリを解放する
		return;
	}
}

//*******************************************************************
// 録音デバイスを閉じる
//*******************************************************************
void CloseWaveInputDevice(void)
{
	int i;

	// 録音を停止する
	waveInStop(hWave);
	waveInReset(hWave);

	// ヘッダを非準備状態にする
	for(i=0; i<MAX_BUFFERS; i++) {//MAX_BUFFERS 8
		waveInUnprepareHeader(hWave, wh[i], sizeof(WAVEHDR));
	}

	waveInClose(hWave);

	//*******************************************************************
	// 割り当てた録音バッファを解放
	//*******************************************************************
	if( hGMemRecBuf != NULL ) GlobalFree( hGMemRecBuf );
	for (i = 0; i < MAX_BUFFERS; i++) {
		wh[i] = NULL;
	}

	bWaveOpened = false;
}

#if 0



//*******************************************************************
// 関数プロトタイプ
//*******************************************************************
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
void DrawGauge(HWND,HBITMAP,DWORD);
void CALLBACK WaveInProc(HWAVEIN,UINT,DWORD,DWORD,DWORD);
void PlotSpectrum(HWND,DWORD);

//*******************************************************************
// MainWndProc - メインウィンドウのメッセージ処理
	  // コンストラクタ
	  case WM_CREATE: {
		TOOLINFO	 ti;
		LPLOGPALETTE lpPal;
		LPDWORD		 dp;
		LPDWORD		 sp;
		int			 i;
		RECT		 r;
		HDC			 hDC;


		// サンプリングバッファを確保
		wavebuf = (BUFTYPE *) GlobalAlloc( GPTR, BUFSZ * sizeof(BUFTYPE) );
		for(i=0; i<MAX_BUFFERS; i++) wh[i] = NULL;

		{
			// "SNDVOL32.EXE" の存在チェック
			LPTSTR result;
			result = (LPTSTR)GlobalAlloc( GMEM_FIXED, 1024 );
			if( (UINT)FindExecutable( TEXT("SNDVOL32.EXE"), NULL, result ) < 32 ) {
				SendMessage(hToolWnd, TB_SETSTATE, IDM_VOLCTRL, TBSTATE_INDETERMINATE);
			}
			GlobalFree( result );
		}
		// 一時停止状態を解除する
		SendMessage(hToolWnd, TB_SETSTATE, IDM_PAUSE, TBSTATE_ENABLED);
		PostMessage(hWnd, WM_COMMAND, IDM_PAUSE, 0);


	  // デストラクタ
	  case WM_DESTROY:
		// プロセスの優先順位を NORMAL に戻す
		// （さもなくばウィンドウが閉じられるまでに時間がかかることがある）
		SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );

		// WAVEデバイスを閉じる
		PostQuitMessage(0);
		break;

	  // 録音バッファが一杯になった時の処理
	  // ツールバーのボタンに応答する処理
	  case WM_COMMAND:
		switch( LOWORD(wParam) ) {

		  // 「画鋲」ボタンの処理
		  case IDM_LOCKWND: {
			if( SendMessage(hToolWnd, TB_GETSTATE, LOWORD(wParam), 0) & TBSTATE_CHECKED ) {
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 1);
				// Windowを“最前面表示”に設定
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				// プロセスの優先順位を HIGH に設定
				SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
			} else {
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 0);
				// “最前面表示”を解除
				SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				// プロセスの優先順位を NORMAL に設定
				SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );
			}
			break;
		  }

		  // 「電源」ボタンの処理
		  /* → 「一時停止」ボタンに変更
		  case IDM_POWER: {
			if( SendMessage(hToolWnd, TB_GETSTATE, LOWORD(wParam), 0) & TBSTATE_CHECKED ) {
				// 電源ＯＮ
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 3);
				if(!bWaveOpened) {
					bWaveOpened = OpenWaveInputDevice(hWnd);
					if(!bWaveOpened) {
						SendMessage(hToolWnd, TB_SETSTATE, LOWORD(wParam),TBSTATE_ENABLED);
						SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 2);
						break;
					}
					StartRecording();
				}
			} else {
				// 電源ＯＦＦ
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 2);
				if(bWaveOpened) {
					CloseWaveInputDevice();
				}
			}
			break;
		  }
		  */

		  // 「一時停止」ボタンの処理


		  // 「録音レベル調整」ボタンの処理
		  case IDM_VOLCTRL:
			break;


//*******************************************************************
// スペクトルをDIBバッファに描画する
//	以下のグローバル変数を使用：
//		BUFTYPE wavebuf[BUFSZ]		音声サンプリングデータ
//		double fnWFilter[BUFSZ/2]	窓関数テーブル
//		int ipTab[2+SQRT_BUFSZ2]	FFT sin/cos table
//		double wTab[BUFSZ/2]		FFT sin/cos table
//		double aFFT[BUFSZ]			FFT data
//		int ipTab2[2+SQRT_BUFSZ2]	FFT sin/cos table
//		double wTab2[BUFSZH/2]		FFT sin/cos table
//		double aFFT2[BUFSZH]		FFT data
//		int logtbl[DRAWDIVS+1]		音階変換用対数テーブル
//		LPBYTE lpBits				DIBバッファイメージ部へのポインタ
//*******************************************************************
void PlotSpectrum(
	HWND hWnd,
	DWORD PlotMode // PM_NOTATE | PM_OSCILLO
) {
	int i, j, s, d;
	double dt;

}


//*******************************************************************
// WaveInコールバック関数
// (注) この関数内では限られたAPIしか呼べず早急に復帰する必要がある
//*******************************************************************




void CS8tunerView::OnRecvolume() 
{
	ShellExecute( NULL, NULL, TEXT("SNDVOL32.EXE"), TEXT("/RECORD"), NULL, SW_SHOWNORMAL );
}
#endif
