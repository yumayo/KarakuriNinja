# pragma once

# include "cinder/app/App.h"
#   ifdef _DEBUG
#       define CONSOLE cinder::app::console()
#   else
#       define CONSOLE 0 && cinder::app::console()
#   endif

# include "cinder/app/MouseEvent.h"
using Mouse = cinder::app::MouseEvent;

# include "cinder/app/KeyEvent.h"
using Key = cinder::app::KeyEvent;

# include "../src/Lib/Input.h"

# define inputs Lib::Input::Geti( )
# define env Lib::Input::Geti( ).GetApp( )