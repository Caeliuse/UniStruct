#include "gapbuffer.h"

#define DEF_GAP_RESERVE ((usize)5)

DEFINE_ARRAY_IMPLEMENTATION(char);

GapBuffer CreateGb(const Array(char) STRING){
	return (GapBuffer){
		.string = CopyArray(char, STRING),
		.gap.begin = 0,
		.gap.size = 0
	};
}

void DestroyGb(GapBuffer* gb){
	DestroyArray(char, &gb->string);
	*gb = EMPTY_GB;
}

GapBuffer CopyGb(const GapBuffer* GB){
	return (GapBuffer){
		.string = CopyArray(char, GB->string),
		.gap = GB->gap
	};
}

bool GbEqual(GapBuffer* A, GapBuffer* B){
	if (GbSize(A) != GbSize(B))
		return false;
	return ArraysEqual(char,
		GbStringMode(A),
		GbStringMode(B)
	);
}

static void MoveGap(GapBuffer* gb, const usize INDEX){
	void ShiftLeft(GapBuffer* gb, usize shift){
		while (shift--){
			const usize BEGIN = gb->gap.begin;
			const usize END = BEGIN + gb->gap.size;
			gb->string.base[END - 1] = gb->string.base[BEGIN - 1];
			--gb->gap.begin;
		}
	}

	void ShiftRight(GapBuffer* gb, usize shift){
		while (shift--){
			const usize BEGIN = gb->gap.begin;
			const usize END = BEGIN + gb->gap.size;
			gb->string.base[BEGIN] = gb->string.base[END];
			++gb->gap.begin;
		}
	}

	if (gb->gap.size == 0 || gb->gap.begin == INDEX)
		gb->gap.begin = INDEX;
	else if (gb->gap.begin < INDEX)
		ShiftRight(gb, INDEX - gb->gap.begin);
	else
		ShiftLeft(gb, gb->gap.begin - INDEX);
}

void PrepareGap(GapBuffer* gb, const usize INDEX, const usize MIN_SIZE){
	MoveGap(gb, INDEX);
	if (gb->gap.size < MIN_SIZE){
		const usize DIFF = MIN_SIZE - gb->gap.size;
		const usize TO_RESERVE = MAX(DIFF, DEF_GAP_RESERVE);
		GbReserve(gb, TO_RESERVE);
	}
}

void GbInsert(GapBuffer* gb, const usize INDEX, const char VALUE){
	PrepareGap(gb, INDEX, 1);

	const usize BEGIN = gb->gap.begin;
	gb->string.base[BEGIN] = VALUE;

	++gb->gap.begin;
	--gb->gap.size;
}

void GbErase(GapBuffer* gb, const usize INDEX){
	GbEraseString(gb, INDEX, 1);
}

void GbAppend(GapBuffer* gb, const char VALUE){
	GbInsert(gb, GbSize(gb), VALUE);
}

void GbPrepend(GapBuffer* gb, const char VALUE){
	GbInsert(gb, 0, VALUE);
}

char GbPop(GapBuffer* gb){
	const usize LAST_INDEX = GbSize(gb) - 1;
	const char LAST_ITEM = GbGet(gb, LAST_INDEX);
	GbErase(gb, LAST_INDEX);
	return LAST_ITEM;
}

char GbPopFront(GapBuffer* gb){
	const char FIRST_ITEM = GbGet(gb, 0);
	GbErase(gb, 0);
	return FIRST_ITEM;
}

void GbInsertString(GapBuffer* gb, const usize INDEX, const Array(char) STRING){
	PrepareGap(gb, INDEX, STRING.size);

	char* gap = gb->string.base + gb->gap.begin;
	memcpy(gap, STRING.base, STRING.size * sizeof(char));

	gb->gap.size -= STRING.size;
	gb->gap.begin += STRING.size;
}

void GbEraseString(GapBuffer* gb, const usize INDEX, const usize SIZE){
	MoveGap(gb, INDEX + SIZE);
	
	gb->gap.size += SIZE;
	gb->gap.begin -= SIZE;
}

usize GbSize(const GapBuffer* GB){
	return GB->string.size - GbReserved(GB);
}

usize GbReserved(const GapBuffer* GB){
	return GB->gap.size;
}

char GbGet(const GapBuffer* GB, const usize INDEX){
	if (INDEX < GB->gap.begin)
		return GB->string.base[INDEX];
	return GB->string.base[INDEX + GB->gap.size];
}

void GbSet(GapBuffer* gb, const usize INDEX, const char VALUE){
	if (INDEX < gb->gap.begin)
		gb->string.base[INDEX] = VALUE;
	gb->string.base[INDEX + gb->gap.size] = VALUE;
}

void GbReserve(GapBuffer* gb, const usize SIZE){
	InsertSubArray(char, &gb->string, gb->gap.begin, SIZE);
	gb->gap.size += SIZE;
}

void GbShrink(GapBuffer* gb){
	Array(char) sub = (Array(char)){
		.base = gb->string.base + gb->gap.begin,
		.size = gb->gap.size
	};
	EraseSubArray(char, &gb->string, sub);
	gb->gap.size = 0;
}

Array(char) GbStringMode(GapBuffer* gb){
	MoveGap(gb, GbSize(gb));
	return WSS(
		gb->string.base,
		GbSize(gb)
	);
}

char* GbCMode(GapBuffer* gb){
	GbAppend(gb, '\0');
	--gb->gap.begin;
	++gb->gap.size;
	return gb->string.base;
}

GapBuffer GbLoadUntil(FILE* file, const char STOP){
	GapBuffer gb = EMPTY_GB;
	for (char c = fgetc(file); c != EOF && c != STOP; c = fgetc(file))
		GbAppend(&gb, c);
	return gb;
}

GapBuffer GbLoad(FILE* file){
	return GbLoadUntil(file, EOF);
}

GapBuffer GbInput(const char* FORMAT, ...){
	{
		va_list args;
	    va_start(args, FORMAT);

	    vprintf(FORMAT, args);

	    va_end(args);
	}

	return GbLoadUntil(stdin, '\n');
}