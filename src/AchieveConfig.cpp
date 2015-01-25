#include "AchieveConfig.h"
#include "Config.h"
#include "Defines.h"

AchieveConfig::AchieveConfig(Config* pConfig)
: m_pConfig(pConfig), m_bNewAchievement(false)
{
}

AchieveConfig::~AchieveConfig()
{
}

bool AchieveConfig::PlayedAGame() const
{
   return m_pConfig->GetGamesPlayed() > 0;
}

bool AchieveConfig::Played10Games() const
{
   return m_pConfig->GetGamesPlayed() >= 10;
}

bool AchieveConfig::WonAGame() const
{
   return m_pConfig->GetGamesWon() > 0;
}

bool AchieveConfig::Won5Games() const
{
   return m_pConfig->GetGamesWon() >= 5;
}

bool AchieveConfig::Win8Colors() const
{
   return m_pConfig->GetWin8Colors() > 0;
}

bool AchieveConfig::Win5Holes() const
{
   return m_pConfig->GetWin5Holes() > 0;
}

bool AchieveConfig::WinLastTry() const
{
   return m_pConfig->GetWinLastTry() > 0;
}

bool AchieveConfig::LookForAchievements(MasterLib mastermind)
{
   int nGamesPlayed = m_pConfig->GetGamesPlayed();
   int nGamesWon = m_pConfig->GetGamesWon();
   int TimesWin8Colors = m_pConfig->GetWin8Colors();
   int TimesWin5Holes = m_pConfig->GetWin5Holes();
   int TimesWinLastTry = m_pConfig->GetWinLastTry();
   bool bNewAchievement = false;

   if( IsMasterGameOver(mastermind) == MASTERLIB_GAME_OVER ) {
      m_pConfig->SetGamesPlayed(m_pConfig->GetGamesPlayed()+1);
      if( nGamesPlayed==0 || nGamesPlayed==9 )
         bNewAchievement = true;
   }

   if( GetMasterWonGame(mastermind) == MASTERLIB_WON_GAME ) {
      m_pConfig->SetGamesWon(m_pConfig->GetGamesWon()+1);
      if( nGamesWon==0 || nGamesWon==4 )
         bNewAchievement = true;
   }

   if( GetMasterWonGame(mastermind) == MASTERLIB_WON_GAME && m_pConfig->GetColors() == 8 ) {
      m_pConfig->SetWin8Colors(m_pConfig->GetWin8Colors()+1);
      if( TimesWin8Colors == 0 )
         bNewAchievement = true;
   }

   if( GetMasterWonGame(mastermind) == MASTERLIB_WON_GAME && m_pConfig->GetHoles() == 5 ) {
      m_pConfig->SetWin5Holes(m_pConfig->GetWin5Holes()+1);
      if( TimesWin5Holes == 0 )
         bNewAchievement = true;
   }

   if( GetMasterWonGame(mastermind) == MASTERLIB_WON_GAME && GetCurrentTry(mastermind)>=m_pConfig->GetTries() ) {
      m_pConfig->SetWinLastTry(m_pConfig->GetWinLastTry()+1);
      if( TimesWinLastTry == 0 )
         bNewAchievement = true;
   }

   if( bNewAchievement )
      m_bNewAchievement = bNewAchievement;
   return bNewAchievement;
}

bool AchieveConfig::GetNewAchievements() const
{
   return m_bNewAchievement;
}

void AchieveConfig::ResetNewAchievements()
{
   m_bNewAchievement = false;
}
