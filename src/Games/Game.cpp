
#include <Games/GameSelector.h>


#define WHOLE_DIR(dir_name) Games/dir_name/dir_name.pkg

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)

$pfile STRINGIZE(WHOLE_DIR(CORE_GAME))
