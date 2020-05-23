#ifndef UICOMMANDMAP_H
#define UICOMMANDMAP_H

#include <QString>
/**
 * @brief The map that summarizes IDs of UI operation
 *        functions including redo and undo.
 */
class UiCommandMap {
public:

    /**
     * @fn UiCommandMap::Id
     * @brief Get id number in QString.
     * @param undoid constexpr data
     * @return Id number in QString
     */
    static const QString Id(int undoid){ return QString::number(undoid); }

    //! Undo ID: 0x10000005 Unassociated Edit Command
    static constexpr int UNDOREDO_EDIT           = 0x10000005;


    //! Undo ID: 0x00000001 (XML Tag: "istack")
    static constexpr int FUNCTIONSELECT          = 0x00000001;


    //! Undo ID: 0x00000100 (XML Tag: "iname")
    static constexpr int I_NAME                  = 0x00000100;

    //! Undo ID: 0x00000101 (XML Tag: "ver")
    static constexpr int I_VERSION               = 0x00000101;

    //! Undo ID: 0x00000102 (XML Tag: "author")
    static constexpr int I_AUTHOR                = 0x00000102;

    //! Undo ID: 0x00000103 (XML Tag: "desc")
    static constexpr int I_DESCRIPTION           = 0x00000103;

    //! Undo ID: 0x00000104 (XML Tag: "finput")
    static constexpr int I_FILEINPUT_BOOL        = 0x00000104;

    //! Undo ID: 0x00000105 (XML Tag: "sinput")
    static constexpr int I_FILEINPUT_SEARCH_BOOL = 0x00000105;

    //! Undo ID: 0x00000106 (XML Tag: "fsname")
    static constexpr int I_FILESEARCH_NAME       = 0x00000106;

    //! Undo ID: 0x00000107 (XML Tag: "rloop")
    static constexpr int I_PROCESS_BOOL_HA1      = 0x00000107;

    //! Undo ID: 0x00000108 (XML Tag: "rlarg")
    static constexpr int I_ARG_IN_ONELOOP_INT    = 0x00000108;

    //! Undo ID: 0x00000109 (XML Tag: "reloop")
    static constexpr int I_RECURSIVE_LOOPMAX_INT = 0x00000109;

    //! Undo ID: 0x00000110 (XML Tag: "max")
    static constexpr int I_PROCESSMAX_INT        = 0x00000110;


    //! Local Tag
    //! Undo ID: 0x00000200 (XML Tag: "lvar")
    static constexpr int L_VARIANT_HA1           = 0x00000200;

    //! Undo ID: 0x00000201 (XML Tag: "localc")
    static constexpr int L_VARIANTCOUNT_INT      = 0x00000201;

    //! Undo ID: 0x00000202 (XML Tag: "lval")
    static constexpr int L_VALUE                = 0x00000202;

    //! Executable Tag
    //! Undo ID: 0x00000300 (XML Tag: "timeout")
    static constexpr int E_TIMEOUT_BOOL_HA1      = 0x00000300;

    //! Undo ID: 0x00000301 (XML Tag: "detach")
    static constexpr int E_DETACH_BOOL           = 0x00000301;

    //! Undo ID: 0x00000302 (XML Tag: "exc")
    static constexpr int E_CMD_HA1               = 0x00000302;

    //! Undo ID: 0x00000303 (XML Tag: "cmdc")
    static constexpr int E_COMMANDCOUNT_INT      = 0x00000303;

    //! Undo ID: 0x00000304 (XML Tag: "dur")
    static constexpr int E_TIMEOUT_INT            = 0x00000304;

    //! Search Tag
    //! Undo ID: 0x00000400 (XML Tag: "sname")
    static constexpr int FS_NAME_HA1             = 0x00000400;

    //! Undo ID: 0x00000401 (XML Tag: "sep")
    static constexpr int FS_SEPARATOR            = 0x00000401;

    //! Undo ID: 0x00000402 (XML Tag: "var")
    static constexpr int FS_VARIANT              = 0x00000402;

    //! Undo ID: 0x00000403 (XML Tag: "output")
    static constexpr int FS_FILEPATH_HA1         = 0x00000403;

    //! Undo ID: 0x00000404 (XML Tag: "fitype")
    static constexpr int FS_WRITEOPTION_INT      = 0x00000404;

    //! Undo ID: 0x00000405 (XML Tag: "radio")
    static constexpr int FS_OUTPUTOPTION_INT    = 0x00000405;


    //! Plugin Tag
    //! Undo ID: 0x00000500 (XML Tag: "plname")
    static constexpr int P_NAME                  = 0x00000500;

    //! Undo ID: 0x00000501 (XML Tag: "plfile")
    static constexpr int P_FILEPATH              = 0x00000501;

    //! Undo ID: 0x00000502 (XML Tag: "plc")
    static constexpr int P_CMD_HA1               = 0x00000502;

    //! Undo ID: 0x00000503 (XML Tag: "pcmdc")
    static constexpr int P_COMMANDCOUNT_INT      = 0x00000503;

    //! profileload Tag
    //! Undo ID: 0x00000600 (XML Tag: "prfile")
    static constexpr int PLOAD_FILEPATH          = 0x00000600;


    //! Undo ID: 0x00001000
    static constexpr int E_ADD_TABLE    = 0x00001000;

    //! Undo ID: 0x00001001
    static constexpr int E_INSERT_TABLE = 0x00001001;

    //! Undo ID: 0x00001002
    static constexpr int E_PASTE_TABLE = 0x00001002;

    //! Undo ID: 0x00001003
    static constexpr int E_EDIT_TABLE   = 0x00001003;

    //! Undo ID: 0x00001004
    static constexpr int E_DELETE_TABLE = 0x00001004;

    //! Undo ID: 0x00001005
    static constexpr int E_CUT_TABLE = 0x00001005;

    //! Undo ID: 0x00001006
    static constexpr int E_SWAP_TABLE   = 0x00001006;

    //! Undo ID: 0x00001007
    static constexpr int E_MOVE_TABLE   = 0x00001007;


    //! Undo ID: 0x00001100
    static constexpr int PL_ADD_TABLE       = 0x00001100;

    //! Undo ID: 0x00001101
    static constexpr int PL_INSERT_TABLE    = 0x00001101;

    //! Undo ID: 0x00001102
    static constexpr int PL_PASTE_TABLE     = 0x00001102;

    //! Undo ID: 0x00001103
    static constexpr int PL_EDIT_TABLE      = 0x00001103;

    //! Undo ID: 0x00001104
    static constexpr int PL_DELETE_TABLE    = 0x00001104;

    //! Undo ID: 0x00001105
    static constexpr int PL_CUT_TABLE       = 0x00001105;

    //! Undo ID: 0x00001106
    static constexpr int PL_SWAP_TABLE      = 0x00001106;

    //! Undo ID: 0x00001107
    static constexpr int PL_MOVE_TABLE      = 0x00001107;

    //! Undo ID: 0x00001108
    static constexpr int PL_ALLUPDATE_TABLE = 0x00001108;


    //! Undo ID: 0x00001200
    static constexpr int LV_ADD    = 0x00001200;

    //! Undo ID: 0x00001201
    static constexpr int LV_INSERT = 0x00001201;

    //! Undo ID: 0x00001202
    static constexpr int LV_EDIT   = 0x00001202;

    //! Undo ID: 0x00001203
    static constexpr int LV_DELETE = 0x00001203;

    //! Undo ID: 0x00001204
    static constexpr int LV_SWAP   = 0x00001204;


    //! Undo ID: 0x00001300
    static constexpr int GV_ADD_TREE            = 0x00001300;

    //! Undo ID: 0x00001301
    static constexpr int GV_EDIT_TREE           = 0x00001301;

    //! Undo ID: 0x00001302
    static constexpr int GV_DEL_TREE            = 0x00001302;

    //! Undo ID: 0x00001303
    static constexpr int GV_INS_TREE            = 0x00001303;

    //! Undo ID: 0x00001304
    static constexpr int GV_SWAP_TREE           = 0x00001304;


    //! Undo ID: 0x00001400
    static constexpr int E_ONLY_SCHEDULER  = 0x00001400;

    //! Undo ID: 0x00001401
    static constexpr int FS_ONLY_SCHEDULER  = 0x00001401;

    //! Undo ID: 0x00001402
    static constexpr int PL_ONLY_SCHEDULER = 0x00001402;

    //! Undo ID: 0x00001403
    static constexpr int PR_ONLY_SCHEDULER = 0x00001403;

};

#endif // UICOMMANDMAP_H
