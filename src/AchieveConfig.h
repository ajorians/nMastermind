#ifndef ACHIEVECONFIG_H
#define ACHIEVECONFIG_H

extern "C"
{
	#include <os.h>
	#include "MastermindLib/MastermindLib.h"
}

class Config;

class AchieveConfig
{
public:
   AchieveConfig(Config* pConfig);
   ~AchieveConfig();

   bool PlayedAGame() const;
   bool Played10Games() const;
   bool WonAGame() const;
   bool Won5Games() const;
   bool Win8Colors() const;
   bool Win5Holes() const;
   bool WinLastTry() const;

   bool LookForAchievements(MasterLib mastermind);

   Config* GetConfig() { return m_pConfig; }

   bool GetNewAchievements() const;
   void ResetNewAchievements();

protected:
   Config*	m_pConfig;
   bool		m_bNewAchievement;
};

#endif

