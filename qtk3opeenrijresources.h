#ifndef QTK3OPEENRIJRESOURCES_H
#define QTK3OPEENRIJRESOURCES_H

#include <array>
#include <QString>

#include "k3opeenrijnames.h"

namespace ribi {
namespace koer {

///The Qt resource paths of all images used by the game.
///All images are compiled into the program via K3OpEenRij.qrc,
///so nothing needs to be written to disk
///(which also makes the game work in a web browser).
struct QtK3OpEenRijResources
{
  ///Colored picture if the girl is played by a human, grey otherwise
  static QString GetPlayerFilename(const bool is_human, const BlackHairedGirl girl);
  static QString GetPlayerFilename(const bool is_human, const BlondeGirl girl);
  static QString GetPlayerFilename(const bool is_human, const RedHairedGirl girl);

  ///The three chosen players, in order red, black, blond
  static std::array<QString, 3> GetPlayerFilenames(
    const bool is_human,
    const BlackHairedGirl black,
    const BlondeGirl blond,
    const RedHairedGirl red
  );

  ///index must be 0, 1 or 2
  static QString GetComputerFilename(const int index);
  static QString GetComputerGreyFilename();
  static QString GetEmptyFilename();
  static QString GetIconFilename();
};

} //~namespace koer
} //~namespace ribi

#endif // QTK3OPEENRIJRESOURCES_H
