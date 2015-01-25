#include "Config.h"
#include "Defines.h"

#define SELECTOR_ANIMATE_DEFAULT        (true)
#define BACKGROUND_MOVES_DEFAULT	(true)
#define HOLES_DEFAULT			(4)
#define COLORS_DEFAULT			(6)
#define TRIES_DEFAULT			(10)

#define GAMES_PLAYED_DEFAULT            (0)
#define GAMES_WON_DEFAULT               (0)
#define WIN_8_COLORS_DEFAULT		(0)
#define WIN_5_HOLES_DEFAULT		(0)
#define WIN_LAST_TRY_DEFAULT		(0)

#define STRINGIFY_CONFIG_ITEM(x) #x

#define READ_BOOL_CONFIG_VAR(identifier) if( strcmp(strName, STRINGIFY_CONFIG_ITEM(Use##identifier) ) == 0 ) {\
   if( strcmp("0", GetValue(m_Archive, "Settings", i)) == 0 ) {\
      m_##identifier = false;\
   } else {\
      m_##identifier = true;\
  }\
}

#define WRITE_BOOL_CONFIG_VAR(identifier) UpdateArchiveEntry(m_Archive, "Settings", STRINGIFY_CONFIG_ITEM(Use##identifier), m_##identifier ? "1" : "0", NULL);

#define READ_INT_CONFIG_VAR(identifier) if( strcmp(strName, STRINGIFY_CONFIG_ITEM(Use##identifier) ) == 0 ) {\
   m_##identifier = atoi( GetValue(m_Archive, "Settings", i));\
}

#define WRITE_INT_CONFIG_VAR(identifier) sprintf(buffer, "%d", m_##identifier);\
   UpdateArchiveEntry(m_Archive, "Settings", STRINGIFY_CONFIG_ITEM(Use##identifier), buffer, NULL);

#define READ_STRING_CONFIG_VAR(identifier) if( strcmp(strName, STRINGIFY_CONFIG_ITEM(Current##identifier) ) == 0 ) {\
   strcpy(m_##identifier, GetValue(m_Archive, "Settings", i));\
}

#define WRITE_STRING_CONFIG_VAR(identifier) UpdateArchiveEntry(m_Archive, "Settings", STRINGIFY_CONFIG_ITEM(Current##identifier), m_##identifier, NULL);

Config::Config()
: m_SelectorMovementAnimate(SELECTOR_ANIMATE_DEFAULT), 
m_BackgroundMoves(BACKGROUND_MOVES_DEFAULT),
m_Holes(HOLES_DEFAULT),
m_Colors(COLORS_DEFAULT),
m_Tries(TRIES_DEFAULT), 
m_GamesPlayed(GAMES_PLAYED_DEFAULT),
m_GamesWon(GAMES_WON_DEFAULT),
m_Win8Colors(WIN_8_COLORS_DEFAULT),
m_Win5Holes(WIN_5_HOLES_DEFAULT),
m_WinLastTry(WIN_LAST_TRY_DEFAULT)
{
   ArchiveCreate(&m_Archive);

   OpenArchiveFile(m_Archive, "MastermindSettings.tns");

   int nSettings = GetNumberArchives(m_Archive, "Settings");
   char strName[MAX_NAME_LENGTH];
   for(int i=0; i<nSettings; i++) {
      strcpy(strName, GetName(m_Archive, "Settings", i));

      READ_BOOL_CONFIG_VAR(SelectorMovementAnimate);
      READ_BOOL_CONFIG_VAR(BackgroundMoves);
      //READ_INT_CONFIG_VAR(CurrentLevel);
      //READ_STRING_CONFIG_VAR(LevelData);
      READ_INT_CONFIG_VAR(Holes);
      READ_INT_CONFIG_VAR(Colors);
      READ_INT_CONFIG_VAR(Tries);

//Achievements
      READ_INT_CONFIG_VAR(GamesPlayed);
      READ_INT_CONFIG_VAR(GamesWon);
      READ_INT_CONFIG_VAR(Win8Colors);
      READ_INT_CONFIG_VAR(Win5Holes);
      READ_INT_CONFIG_VAR(WinLastTry);
   }
}

Config::~Config()
{
   char buffer[8];
   ArchiveSetBatchMode(m_Archive, ARCHIVE_ENABLE_BATCH);
   WRITE_BOOL_CONFIG_VAR(SelectorMovementAnimate);
   WRITE_BOOL_CONFIG_VAR(BackgroundMoves);
   //char buffer[8];
   //WRITE_INT_CONFIG_VAR(CurrentLevel);
   //WRITE_STRING_CONFIG_VAR(LevelData);
   WRITE_INT_CONFIG_VAR(Holes);
   WRITE_INT_CONFIG_VAR(Colors);
   WRITE_INT_CONFIG_VAR(Tries);

//Achievements
   WRITE_INT_CONFIG_VAR(GamesPlayed);
   WRITE_INT_CONFIG_VAR(GamesWon);
   WRITE_INT_CONFIG_VAR(Win8Colors);
   WRITE_INT_CONFIG_VAR(Win5Holes);
   WRITE_INT_CONFIG_VAR(WinLastTry);

   ArchiveSetBatchMode(m_Archive, ARCHIVE_DISABLE_BATCH);

   ArchiveFree(&m_Archive);
}


