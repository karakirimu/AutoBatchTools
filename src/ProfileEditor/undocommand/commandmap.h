#ifndef COMMANDMAP_H
#define COMMANDMAP_H

class CommandMap {
public:

    //! Undo ID: 0x10000000 Unassociated Add Command
    static constexpr int UNDOREDO_ADD               = 0x10000000;

    //! Undo ID: 0x10000001 Unassociated Delete Command
    static constexpr int UNDOREDO_DELETE            = 0x10000001;

    //! Undo ID: 0x10000002 Unassociated Insert Command
    static constexpr int UNDOREDO_INSERT            = 0x10000002;

    //! Undo ID: 0x10000003 Unassociated Swap Command
    static constexpr int UNDOREDO_SWAP              = 0x10000003;

    //! Undo ID: 0x10000004 Unassociated Move Command
    static constexpr int UNDOREDO_MOVE              = 0x10000004;
};

#endif // COMMANDMAP_H
