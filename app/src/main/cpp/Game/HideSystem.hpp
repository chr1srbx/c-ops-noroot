namespace hide_system {

    void* (*oOnSendMessage)(monoString* message, int type, bool commandUIUsed);
    void* OnSendMessage(monoString* message, int type, bool commandUIUsed)
    {
        if(strcmp(message->getString().c_str(), "/hide") == 0) {
            menuOpen = false;
            return oOnSendMessage(message, type, commandUIUsed);
        }
        if(strcmp(message->getString().c_str(), "/show") == 0) {
            menuOpen = true;
            return oOnSendMessage(message, type, commandUIUsed);
        }
        return oOnSendMessage(message, type, commandUIUsed);
    }

    void init()
    {
        hook("0x19EEEDC", OnSendMessage, oOnSendMessage);
    }
}