
#include <Games/GameSelector.h>


#define WHOLE_DIR(dir_name) ../src/Games/dir_name/gameExport.pkg

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)

//$#include CORE_GAME_HEADER
$pfile STRINGIZE(WHOLE_DIR(CORE_GAME))
