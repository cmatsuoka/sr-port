precision mediump float;

uniform vec3	iResolution;		// viewport resolution (in pixels)
uniform float	iGlobalTime;		// shader playback time (in seconds)



#define DPII (3.1415926535*2.0)

// The original code had a "pompota" function that would toggle the VGA
// horizontal scroll offset each frame between values of 0 and 4, as well
// as toggle the vertical start position between 60 and 61. This effect
// doesn't seem to  work that well now, so it has been disabled.

#define POMPOTA 0

int GetPerPlasmaTime(int t)
{
	// Get the time since this plasma started using the total time.
	// Times for each plasma are (from the timetable array): 723, 1491,
	// 1875, 2259, 2278.  (64 is added to each time because it takes 64
	// frames to drop the current plasma offscreen before switching to
	// the new one.)

	if (t < 723+64) {

	} else if (t < 1491+64) {
		t -= 723+64;
	} else if (t < 1875+64) {
		t -= 1491+64;
	} else if (t < 2259+64) {
		t -= 1875+64;
	} else if (t < 2278+64) {
		t -= 2259+64;
	} else {
		t = 0;
	}

	return t;
}

int GetPlasmaIndex(int t)
{
	int nPlasmaIndex = 0;

	if (t < 723+64) {
		nPlasmaIndex = 0;
	} else if (t < 1491+64) {
		nPlasmaIndex = 1;
	} else if (t < 1875+64) {
		nPlasmaIndex = 2;
	} else if (t < 2259+64) {
		nPlasmaIndex = 3;
	} else if (t < 2278+64) {
		nPlasmaIndex = 4;
	} else {
		nPlasmaIndex = 5;
	}

	return nPlasmaIndex;
}

int GetXOffset(int t)
{
#if POMPOTA == 1
	int nResult = 4;
	float fT = float(t);
	int nMod = int(mod(fT, 2.0));

	if (nMod != 0) {
		nResult = 0;
	}
	return nResult;

#else //POMPOTA == 1
		return 0;
#endif //POMPOTA == 1
}

int GetYOffset(int t)
{
	int nResult = 60;
	bool bDrop = false;

	if ( (t >= 723) && (t < (723+64)) ) {
		t -= 723;
		bDrop = true;
	} else if ( (t >= 1491) && (t < (1491+64)) ) {
		t -= 1491;
		bDrop = true;
	} else if ( (t >= 1875) && (t < (1875+64)) ) {
		t -= 1875;
		bDrop = true;
	} else if ( (t >= 2259) && (t < (2259+64)) ) {
		t -= 2259;
		bDrop = true;
	}

	if (bDrop) {
		nResult = t * t / 4 * 43 / 128 + 60;
	}
#if POMPOTA == 1
	else {
		float fT = float(t);
		int nMod = int(mod(fT, 2.0));
		if (nMod != 0) {
			nResult = 61;
		}
	}
#endif //POMPOTA == 1

	return nResult;
}

float FadeColourComponent(float c, int t)
{
	float fResult = c;
	float fProportion = 0.0;

	if ((t >= 0) && (t < 128)) {
		// For plasma 1, fade from white to the palette.
		fProportion = float(t) / 127.0;
		fResult = (fProportion * c) + ((1.0 - fProportion) * 1.0);
	} else if ( (t >= (723+64)) && (t < (723+64+32)) ) {
		// For plasma 2, fade from black to the palette.
		fProportion = float(t-723-64) / 31.0;
		fResult = (fProportion * c) + ((1.0 - fProportion) * 0.0);
	} else if ( (t >= (1491+64)) && (t < (1491+64+32)) ) {
		// For plasma 3, fade from black to the palette.
		fProportion = float(t-1491-64) / 31.0;
		fResult = (fProportion * c) + ((1.0 - fProportion) * 0.0);
	} else if ( (t >= (1875+64)) && (t < (1875+64+32)) ) {
		// For plasma 4, fade from black to the palette.
		fProportion = float(t-1875-64) / 31.0;
		fResult = (fProportion * c) + ((1.0 - fProportion) * 0.0);
	}

	return fResult;
}

int GetInitialL1(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 1000;
	} else if (nPlasmaIndex == 1) {
		return 1000;
	} else if (nPlasmaIndex == 2) {
		return 3500;
	} else if (nPlasmaIndex == 3) {
		return 1000;
	} else if (nPlasmaIndex == 4) {
		return 1000;
	} else {
		return 1000;
	}
}
int GetInitialL2(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 2000;
	} else if (nPlasmaIndex == 1) {
		return 2000;
	} else if (nPlasmaIndex == 2) {
		return 1000;
	} else if (nPlasmaIndex == 3) {
		return 2000;
	} else if (nPlasmaIndex == 4) {
		return 2000;
	} else {
		return 2000;
	}
}

int GetInitialL3(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 3000;
	} else if (nPlasmaIndex == 1) {
		return 4000;
	} else if (nPlasmaIndex == 2) {
		return 3000;
	} else if (nPlasmaIndex == 3) {
		return 3000;
	} else if (nPlasmaIndex == 4) {
		return 3000;
	} else {
		return 3000;
	}
}

int GetInitialL4(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 4000;
	} else if (nPlasmaIndex == 1) {
		return 4000;
	} else if (nPlasmaIndex == 2) {
		return 1000;
	} else if (nPlasmaIndex == 3) {
		return 4000;
	} else if (nPlasmaIndex == 4) {
		return 4000;
	} else {
		return 4000;
	}
}

int GetInitialK1(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 3500;
	} else if (nPlasmaIndex == 1) {
		return 1500;
	} else if (nPlasmaIndex == 2) {
		return 3500;
	} else if (nPlasmaIndex == 3) {
		return 3500;
	} else if (nPlasmaIndex == 4) {
		return 3500;
	} else {
		return 3500;
	}
}

int GetInitialK2(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 2300;
	} else if (nPlasmaIndex == 1) {
		return 2300;
	} else if (nPlasmaIndex == 2) {
		return 3300;
	} else if (nPlasmaIndex == 3) {
		return 2300;
	} else if (nPlasmaIndex == 4) {
		return 2300;
	} else {
		return 2300;
	}
}

int GetInitialK3(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 3900;
	} else if (nPlasmaIndex == 1) {
		return 3900;
	} else if (nPlasmaIndex == 2) {
		return 2900;
	} else if (nPlasmaIndex == 3) {
		return 3900;
	} else if (nPlasmaIndex == 4) {
		return 3900;
	} else {
		return 3900;
	}
}

int GetInitialK4(int nPlasmaIndex)
{
	if (nPlasmaIndex == 0) {
		return 3670;
	} else if (nPlasmaIndex == 1) {
		return 1670;
	} else if (nPlasmaIndex == 2) {
		return 2670;
	} else if (nPlasmaIndex == 3) {
		return 3670;
	} else if (nPlasmaIndex == 4) {
		return 3670;
	} else {
		return 3670;
	}
}

int GetC1(int nXOffset0or1, int t)
{
	int nPlasmaIndex = GetPlasmaIndex(t);
	t = GetPerPlasmaTime(t);

	int nResult = 0;
	if (nXOffset0or1 == 0) {
		nResult = GetInitialK1(nPlasmaIndex) + (-3 * t);
	} else {
		nResult = GetInitialL1(nPlasmaIndex) + (-1 * t);
	}
	
	float fResult = float(nResult);
	fResult = mod(fResult, 4096.0);

	if (fResult < 0.0) {
		fResult += 4096.0;
	}
	nResult = int(fResult);

	return nResult;
}

int GetC2(int nXOffset0or1, int t)
{
	int nPlasmaIndex = GetPlasmaIndex(t);
	t = GetPerPlasmaTime(t);

	int nResult = 0;
	if (nXOffset0or1 == 0) {
		nResult = GetInitialK2(nPlasmaIndex) + (-2 * t);
	} else {
		nResult = GetInitialL2(nPlasmaIndex) + (-2 * t);
	}
	
	float fResult = float(nResult);
	fResult = mod(fResult, 4096.0);

	if (fResult < 0.0) {
		fResult += 4096.0;
	}
	nResult = int(fResult);

	return nResult;
}

int GetC3(int nXOffset0or1, int t)
{
	int nPlasmaIndex = GetPlasmaIndex(t);
	t = GetPerPlasmaTime(t);

	int nResult = 0;
	if (nXOffset0or1 == 0) {
		nResult = GetInitialK3(nPlasmaIndex) + (1 * t);
	} else {
		nResult = GetInitialL3(nPlasmaIndex) + (2 * t);
	}
	
	float fResult = float(nResult);
	fResult = mod(fResult, 4096.0);

	if (fResult < 0.0) {
		fResult += 4096.0;
	}
	nResult = int(fResult);

	return nResult;
}

int GetC4(int nXOffset0or1, int t)
{
	int nPlasmaIndex = GetPlasmaIndex(t);
	t = GetPerPlasmaTime(t);

	int nResult = 0;
	if (nXOffset0or1 == 0) {
		nResult = GetInitialK4(nPlasmaIndex) + (2 * t);
	} else {
		nResult = GetInitialL4(nPlasmaIndex) + (3 * t);
	}
	
	float fResult = float(nResult);
	fResult = mod(fResult, 4096.0);

	if (fResult < 0.0) {
		fResult += 4096.0;
	}
	nResult = int(fResult);

	return nResult;
}

float Palette0_GetRed(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = float(nIndex);
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
		a = 63.0 - a;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = 0.0;
	} else {
		a = float(nIndex) - 192.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette0_GetGreen(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = 0.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = 0.0;
	} else {
		a = 0.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette0_GetBlue(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = 0.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
	} else {
		a = float(nIndex) - 192.0;
		a = 63.0 - a;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette1_GetRed(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = float(nIndex);
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
		a = 63.0 - a;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = 0.0;
	} else {
		a = float(nIndex) - 192.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette1_GetGreen(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = 0.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
	} else {
		a = 63.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette1_GetBlue(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
		a = 63.0 - a;
	} else {
		a = float(nIndex) - 192.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette2_GetRed(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
		a = 63.0 - a;
	} else {
		a = 0.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 2.0;
	fResult /= 63.0;

	return fResult;
}

float Palette2_GetGreen(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
		a = 63.0 - a;
	} else {
		a = 0.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 2.0;
	fResult /= 63.0;

	return fResult;
}
float Palette2_GetBlue(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
		a = 63.0 - a;
	} else {
		a = 0.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 2.0;
	fResult /= 63.0;

	return fResult;
}

float Palette3_GetRed(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = float(nIndex);
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = 63.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
		a = 63.0 - a;
	} else {
		a = 0.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}
float Palette3_GetGreen(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = float(nIndex) - 128.0;
		a = 63.0 - a;
	} else {
		a = 0.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette3_GetBlue(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 64)) {
		a = 0.0;
	} else if ((nIndex >= 64) && (nIndex < 128)) {
		a = float(nIndex) - 64.0;
	} else if ((nIndex >= 128) && (nIndex < 192)) {
		a = 63.0;
	} else {
		a = 63.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette4_GetRed(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 75)) {
		a = float(nIndex);
		a = 63.0 - a * 64.0 / 75.0;
	} else if ((nIndex >= 75) && (nIndex < 181)) {
		return 0.0;
	} else {
		a = float(nIndex) - 181.0;
		a = (a * 64.0 / 75.0) * 8.0 / 10.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette4_GetGreen(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 75)) {
		a = float(nIndex);
		a = 63.0 - a * 64.0 / 75.0;
	} else if ((nIndex >= 75) && (nIndex < 181)) {
		return 0.0;
	} else {
		a = float(nIndex) - 181.0;
		a = (a * 64.0 / 75.0) * 9.0 / 10.0;
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette4_GetBlue(int nIndex)
{
	float a = 0.0;

	if ((nIndex >= 0) && (nIndex < 75)) {
		a = float(nIndex);
		a = 63.0 - a * 64.0 / 75.0;
	} else if ((nIndex >= 75) && (nIndex < 181)) {
		return 0.0;
	} else {
		a = float(nIndex) - 181.0;
		a = (a * 64.0 / 75.0);
	}

	float fResult = cos(a*(3.1415926535*2.0)/128.0+3.1415926535)*31.0+32.0;
	fResult /= 63.0;

	return fResult;
}

float Palette_GetRed(int nIndex, int t)
{
	int nPlasmaIndex = GetPlasmaIndex(t);
	t = GetPerPlasmaTime(t);
	float fResult = 0.0;

	if (nPlasmaIndex == 0) {
		fResult = Palette0_GetRed(nIndex);
	} else if (nPlasmaIndex == 1) {
		fResult = Palette1_GetRed(nIndex);
	} else if (nPlasmaIndex == 2) {
		fResult = Palette2_GetRed(nIndex);
	} else if (nPlasmaIndex == 3) {
		fResult = Palette3_GetRed(nIndex);
	} else if (nPlasmaIndex == 4) {
		fResult = Palette4_GetRed(nIndex);
	}

	return fResult;
}

float Palette_GetGreen(int nIndex, int t)
{
	int nPlasmaIndex = GetPlasmaIndex(t);
	t = GetPerPlasmaTime(t);
	float fResult = 0.0;

	if (nPlasmaIndex == 0) {
		fResult = Palette0_GetGreen(nIndex);
	} else if (nPlasmaIndex == 1) {
		fResult = Palette1_GetGreen(nIndex);
	} else if (nPlasmaIndex == 2) {
		fResult = Palette2_GetGreen(nIndex);
	} else if (nPlasmaIndex == 3) {
		fResult = Palette3_GetGreen(nIndex);
	} else if (nPlasmaIndex == 4) {
		fResult = Palette4_GetGreen(nIndex);
	}

	return fResult;
}
float Palette_GetBlue(int nIndex, int t)
{
	int nPlasmaIndex = GetPlasmaIndex(t);
	t = GetPerPlasmaTime(t);
	float fResult = 0.0;

	if (nPlasmaIndex == 0) {
		fResult = Palette0_GetBlue(nIndex);
	} else if (nPlasmaIndex == 1) {
		fResult = Palette1_GetBlue(nIndex);
	} else if (nPlasmaIndex == 2) {
		fResult = Palette2_GetBlue(nIndex);
	} else if (nPlasmaIndex == 3) {
		fResult = Palette3_GetBlue(nIndex);
	} else if (nPlasmaIndex == 4) {
		fResult = Palette4_GetBlue(nIndex);
	}

	return fResult;
}

void main(void)
{
	// t is an integer frame count.
	int t = int(iGlobalTime * 60.0);

	// Wrap t when it exceeds 2323, so that the plasmas will repeat.
	float fT = float(t);
	t = int(mod(fT, 2323.0));

	// fPlasmaX is a float from 0 to 319,
	// fPlasmaY is a float from 0 to 399.
	float fPlasmaX = (gl_FragCoord.x / iResolution.x) * 319.0;
	float fPlasmaY = (gl_FragCoord.y / iResolution.y) * 399.0;
	fPlasmaY = 399.0 - fPlasmaY;

	fPlasmaY -= float(GetYOffset(t));
	fPlasmaX += float(GetXOffset(t));

	float fRed = 0.0;
	float fGreen = 0.0;
	float fBlue = 0.0;

	if ( (fPlasmaY >= 0.0) && (fPlasmaY < 280.0) ) {
		float fScreenX = gl_FragCoord.x;
		float fScreenY = gl_FragCoord.y;
		float ah = 0.0;
		float ccc = fPlasmaX / 4.0;
		float fOffset = 0.0;
		float bx = 0.0;

		int nXOffset0or1 = int(mod(fScreenX, 2.0));
		// On every odd row, toggle the x offset. This causes different plasma constants to be
		// read for these pixels, achieving a chequered overlay of two plasmas.
		int xor = int(mod(fScreenY, 2.0));
		if (xor == 1) {
			if (nXOffset0or1 == 0) {
				nXOffset0or1 = 1;
			} else {
				nXOffset0or1 = 0;
			}
		}

		float c1 = float(GetC1(nXOffset0or1, t));
		float c2 = float(GetC2(nXOffset0or1, t));
		float c3 = float(GetC3(nXOffset0or1, t));
		float c4 = float(GetC4(nXOffset0or1, t));

		fOffset = ((fPlasmaY * 2.0) + (c2 * 2.0) - (ccc * 8.0) + (80.0 * 8.0)) / 2.0;
		bx = ((sin(fOffset*DPII/4096.0)*55.0+sin(fOffset*DPII/4096.0*4.0)*5.0+sin(fOffset*DPII/4096.0*17.0)*3.0+64.0)*16.0);

		fOffset = bx + c1 + (ccc * 8.0);
		ah = (sin(fOffset*DPII/4096.0)*55.0+sin(fOffset*DPII/4096.0*6.0)*5.0+sin(fOffset*DPII/4096.0*21.0)*4.0+64.0);

		fOffset = ((fPlasmaY * 2.0) + (c4 * 2.0) + (ccc * 32.0)) / 2.0;
		bx = ((sin(fOffset*DPII/4096.0)*55.0+sin(fOffset*DPII/4096.0*5.0)*8.0+sin(fOffset*DPII/4096.0*15.0)*2.0+64.0)*8.0);

		fOffset = bx + (fPlasmaY * 2.0) + c3 - (ccc * 4.0) + (80.0 * 4.0);
		ah += (sin(fOffset*DPII/4096.0)*55.0+sin(fOffset*DPII/4096.0*6.0)*5.0+sin(fOffset*DPII/4096.0*21.0)*4.0+64.0);

		int nPaletteIndex = int(ah);

		fRed = Palette_GetRed(nPaletteIndex, t);
		fGreen = Palette_GetGreen(nPaletteIndex, t);
		fBlue = Palette_GetBlue(nPaletteIndex, t);
	}

	fRed = FadeColourComponent(fRed, t);
	fGreen = FadeColourComponent(fGreen, t);
	fBlue = FadeColourComponent(fBlue, t);

	gl_FragColor = vec4(fRed, fGreen, fBlue, 1.0);
}

