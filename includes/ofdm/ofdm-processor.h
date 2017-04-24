#
/*
 *    Copyright (C) 2013 .. 2017
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Programming
 *
 *    This file is part of the Qt-DAB program
 *    Qt-DAB is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    Qt-DAB is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Qt-DAB; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#
#ifndef	__OFDM_PROCESSOR__
#define	__OFDM_PROCESSOR__
/*
 *
 */
#include	"dab-constants.h"
#include	<QThread>
#include	<QObject>
#include	"stdint.h"
#include	<sndfile.h>
#include	"phasereference.h"
#include	"ofdm-decoder.h"
#include	"virtual-input.h"
#include	"ringbuffer.h"
#ifdef	TII_ATTEMPT
#include	"tii_detector.h"
#endif
//
//	Note:
//	It was found that enlarging the buffersize to e.g. 8192
//	cannot be handled properly by the underlying system.
#define	DUMPSIZE		4096

class	RadioInterface;
class	common_fft;
class	ficHandler;
class	mscHandler;
class	dabParams;

class ofdmProcessor: public QThread {
Q_OBJECT
public:
		ofdmProcessor  	(RadioInterface *,
	                         virtualInput *,
	                         uint8_t,
	                         mscHandler *,
	                         ficHandler *,
	                         int16_t,
	                         uint8_t
#ifdef	HAVE_SPECTRUM
	                        ,RingBuffer<DSPCOMPLEX>	*,
	                         RingBuffer<DSPCOMPLEX>	*
#endif
	                        );
		~ofdmProcessor	(void);
	void		reset			(void);
	void		stop			(void);
	void		setOffset		(int32_t);
	void		coarseCorrectorOn	(void);
	void		coarseCorrectorOff	(void);
	void		startDumping		(SNDFILE *);
	void		stopDumping		(void);
	void		set_scanMode		(bool);
	void		set_tiiSwitch		(bool);
	void		set_tiiCoordinates	(void);
private:
	virtualInput	*theRig;
	dabParams	params;
	RadioInterface	*myRadioInterface;
	ficHandler	*my_ficHandler;
	bool		tiiSwitch;
	bool		tiiCoordinates;
	int16_t		tiiBuffers;
#ifdef	TII_ATTEMPT
	bool		tiiFound;
        int16_t		tiiCount;
	TII_Detector	my_TII_Detector;
#endif
	int16_t		attempts;
	bool		scanMode;
	bool		running;
	bool		dumping;
	int16_t		dumpIndex;
	int16_t		dumpScale;
	int16_t		dumpBuffer [DUMPSIZE];
	SNDFILE		*dumpFile;
	int32_t		T_null;
	int32_t		T_u;
	int32_t		T_s;
	int32_t		T_g;
	int32_t		T_F;
	int32_t		nrBlocks;
	int32_t		carriers;
	int32_t		carrierDiff;
	float		sLevel;
	DSPCOMPLEX	*dataBuffer;
	int32_t		FreqOffset;
	DSPCOMPLEX	*oscillatorTable;
	int32_t		localPhase;
	int16_t		fineCorrector;
	int32_t		coarseCorrector;

	uint8_t		freqsyncMethod;
	bool		f2Correction;
	int32_t		tokenCount;
	DSPCOMPLEX	*ofdmBuffer;
	uint32_t	ofdmBufferIndex;
	uint32_t	ofdmSymbolCount;
	phaseReference	phaseSynchronizer;
	ofdmDecoder	my_ofdmDecoder;
	DSPFLOAT	avgCorr;
	float		*correlationVector;
	float		*refArg;
	int32_t		sampleCnt;
	int32_t		inputSize;
	int32_t		inputPointer;
	DSPCOMPLEX	getSample	(int32_t);
	void		getSamples	(DSPCOMPLEX *, int16_t, int32_t);
virtual	void		run		(void);
	int32_t		bufferContent;
	bool		isReset;
	int16_t		processBlock_0	(DSPCOMPLEX *);
	void		processNULL	(DSPCOMPLEX *);
	int16_t		getMiddle	(DSPCOMPLEX *);
	common_fft	*fft_handler;
	DSPCOMPLEX	*fft_buffer;
#ifdef  HAVE_SPECTRUM
        RingBuffer<DSPCOMPLEX> *spectrumBuffer;
        int32_t         bufferSize;
        int32_t         localCounter;
        DSPCOMPLEX      *localBuffer;
signals:
        void            showSpectrum            (int);
#endif
signals:
	void		show_fineCorrector	(int);
	void		show_coarseCorrector	(int);
	void		setSynced		(char);
	void		No_Signal_Found		(void);
	void		setSyncLost		(void);
};
#endif

