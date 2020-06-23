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
// CS8tunerView �N���X�̕`��

//*******************************************************************
// �^���f�o�C�X���J�� - ����������TRUE��Ԃ�
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
// �^���J�n
//*******************************************************************
//HGLOBAL			hGMemRecBuf;
void StartRecording(void)
{
	MMRESULT rc;
	int		 i;
	LPSTR	 h;

	//*******************************************************************
	// �^���o�b�t�@���m�ۂ���
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

	// �o�b�t�@�u���b�N���������āA���̓L���[�ɒǉ�����
	for (i=0; i<MAX_BUFFERS; i++) {
		rc = waveInPrepareHeader(hWave, wh[i], sizeof(WAVEHDR));

		// ���̓L���[�Ƀo�b�t�@��ǉ�����
		if (rc == MMSYSERR_NOERROR)
			rc = waveInAddBuffer(hWave, wh[i], sizeof(WAVEHDR));

	}

	// �^�����J�n����
	if (rc == MMSYSERR_NOERROR)
		rc = waveInStart(hWave);

	if (rc != MMSYSERR_NOERROR) {
		CloseWaveInputDevice();	 // ���蓖�Ă�ꂽ���������������
		return;
	}
}

//*******************************************************************
// �^���f�o�C�X�����
//*******************************************************************
void CloseWaveInputDevice(void)
{
	int i;

	// �^�����~����
	waveInStop(hWave);
	waveInReset(hWave);

	// �w�b�_��񏀔���Ԃɂ���
	for(i=0; i<MAX_BUFFERS; i++) {//MAX_BUFFERS 8
		waveInUnprepareHeader(hWave, wh[i], sizeof(WAVEHDR));
	}

	waveInClose(hWave);

	//*******************************************************************
	// ���蓖�Ă��^���o�b�t�@�����
	//*******************************************************************
	if( hGMemRecBuf != NULL ) GlobalFree( hGMemRecBuf );
	for (i = 0; i < MAX_BUFFERS; i++) {
		wh[i] = NULL;
	}

	bWaveOpened = false;
}

#if 0



//*******************************************************************
// �֐��v���g�^�C�v
//*******************************************************************
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
void DrawGauge(HWND,HBITMAP,DWORD);
void CALLBACK WaveInProc(HWAVEIN,UINT,DWORD,DWORD,DWORD);
void PlotSpectrum(HWND,DWORD);

//*******************************************************************
// MainWndProc - ���C���E�B���h�E�̃��b�Z�[�W����
	  // �R���X�g���N�^
	  case WM_CREATE: {
		TOOLINFO	 ti;
		LPLOGPALETTE lpPal;
		LPDWORD		 dp;
		LPDWORD		 sp;
		int			 i;
		RECT		 r;
		HDC			 hDC;


		// �T���v�����O�o�b�t�@���m��
		wavebuf = (BUFTYPE *) GlobalAlloc( GPTR, BUFSZ * sizeof(BUFTYPE) );
		for(i=0; i<MAX_BUFFERS; i++) wh[i] = NULL;

		{
			// "SNDVOL32.EXE" �̑��݃`�F�b�N
			LPTSTR result;
			result = (LPTSTR)GlobalAlloc( GMEM_FIXED, 1024 );
			if( (UINT)FindExecutable( TEXT("SNDVOL32.EXE"), NULL, result ) < 32 ) {
				SendMessage(hToolWnd, TB_SETSTATE, IDM_VOLCTRL, TBSTATE_INDETERMINATE);
			}
			GlobalFree( result );
		}
		// �ꎞ��~��Ԃ���������
		SendMessage(hToolWnd, TB_SETSTATE, IDM_PAUSE, TBSTATE_ENABLED);
		PostMessage(hWnd, WM_COMMAND, IDM_PAUSE, 0);


	  // �f�X�g���N�^
	  case WM_DESTROY:
		// �v���Z�X�̗D�揇�ʂ� NORMAL �ɖ߂�
		// �i�����Ȃ��΃E�B���h�E��������܂łɎ��Ԃ������邱�Ƃ�����j
		SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );

		// WAVE�f�o�C�X�����
		PostQuitMessage(0);
		break;

	  // �^���o�b�t�@����t�ɂȂ������̏���
	  // �c�[���o�[�̃{�^���ɉ������鏈��
	  case WM_COMMAND:
		switch( LOWORD(wParam) ) {

		  // �u��e�v�{�^���̏���
		  case IDM_LOCKWND: {
			if( SendMessage(hToolWnd, TB_GETSTATE, LOWORD(wParam), 0) & TBSTATE_CHECKED ) {
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 1);
				// Window���g�őO�ʕ\���h�ɐݒ�
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				// �v���Z�X�̗D�揇�ʂ� HIGH �ɐݒ�
				SetPriorityClass( GetCurrentProcess(), HIGH_PRIORITY_CLASS );
			} else {
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 0);
				// �g�őO�ʕ\���h������
				SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				// �v���Z�X�̗D�揇�ʂ� NORMAL �ɐݒ�
				SetPriorityClass( GetCurrentProcess(), NORMAL_PRIORITY_CLASS );
			}
			break;
		  }

		  // �u�d���v�{�^���̏���
		  /* �� �u�ꎞ��~�v�{�^���ɕύX
		  case IDM_POWER: {
			if( SendMessage(hToolWnd, TB_GETSTATE, LOWORD(wParam), 0) & TBSTATE_CHECKED ) {
				// �d���n�m
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
				// �d���n�e�e
				SendMessage(hToolWnd, TB_CHANGEBITMAP, LOWORD(wParam), 2);
				if(bWaveOpened) {
					CloseWaveInputDevice();
				}
			}
			break;
		  }
		  */

		  // �u�ꎞ��~�v�{�^���̏���


		  // �u�^�����x�������v�{�^���̏���
		  case IDM_VOLCTRL:
			break;


//*******************************************************************
// �X�y�N�g����DIB�o�b�t�@�ɕ`�悷��
//	�ȉ��̃O���[�o���ϐ����g�p�F
//		BUFTYPE wavebuf[BUFSZ]		�����T���v�����O�f�[�^
//		double fnWFilter[BUFSZ/2]	���֐��e�[�u��
//		int ipTab[2+SQRT_BUFSZ2]	FFT sin/cos table
//		double wTab[BUFSZ/2]		FFT sin/cos table
//		double aFFT[BUFSZ]			FFT data
//		int ipTab2[2+SQRT_BUFSZ2]	FFT sin/cos table
//		double wTab2[BUFSZH/2]		FFT sin/cos table
//		double aFFT2[BUFSZH]		FFT data
//		int logtbl[DRAWDIVS+1]		���K�ϊ��p�ΐ��e�[�u��
//		LPBYTE lpBits				DIB�o�b�t�@�C���[�W���ւ̃|�C���^
//*******************************************************************
void PlotSpectrum(
	HWND hWnd,
	DWORD PlotMode // PM_NOTATE | PM_OSCILLO
) {
	int i, j, s, d;
	double dt;

}


//*******************************************************************
// WaveIn�R�[���o�b�N�֐�
// (��) ���̊֐����ł͌���ꂽAPI�����Ăׂ����}�ɕ��A����K�v������
//*******************************************************************




void CS8tunerView::OnRecvolume() 
{
	ShellExecute( NULL, NULL, TEXT("SNDVOL32.EXE"), TEXT("/RECORD"), NULL, SW_SHOWNORMAL );
}
#endif
