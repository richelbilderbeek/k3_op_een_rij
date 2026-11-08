#include "qtk3opeenrijresources.h"

#include <cassert>

namespace {

QString ToFilename(const std::string& name, const bool is_human)
{
  return QString(":/images/K3OpEenRij")
    + QString::fromStdString(name)
    + (is_human ? "" : "Grey")
    + ".png";
}

} //~namespace

QString ribi::koer::QtK3OpEenRijResources::GetPlayerFilename(
  const bool is_human, const BlackHairedGirl girl)
{
  return ToFilename(ToStr(girl), is_human);
}

QString ribi::koer::QtK3OpEenRijResources::GetPlayerFilename(
  const bool is_human, const BlondeGirl girl)
{
  return ToFilename(ToStr(girl), is_human);
}

QString ribi::koer::QtK3OpEenRijResources::GetPlayerFilename(
  const bool is_human, const RedHairedGirl girl)
{
  return ToFilename(ToStr(girl), is_human);
}

std::array<QString, 3> ribi::koer::QtK3OpEenRijResources::GetPlayerFilenames(
  const bool is_human,
  const BlackHairedGirl black,
  const BlondeGirl blond,
  const RedHairedGirl red)
{
  return {
    GetPlayerFilename(is_human, red),
    GetPlayerFilename(is_human, black),
    GetPlayerFilename(is_human, blond)
  };
}

QString ribi::koer::QtK3OpEenRijResources::GetComputerFilename(const int index)
{
  assert(index >= 0 && index < 3);
  return QString(":/images/K3OpEenRijComputer%1.png").arg(index + 1);
}

QString ribi::koer::QtK3OpEenRijResources::GetComputerGreyFilename()
{
  return ":/images/K3OpEenRijComputerGrey.png";
}

QString ribi::koer::QtK3OpEenRijResources::GetEmptyFilename()
{
  return ":/images/K3OpEenRijEmpty2.png";
}

QString ribi::koer::QtK3OpEenRijResources::GetIconFilename()
{
  return ":/images/K3OpEenRijIcon.png";
}
