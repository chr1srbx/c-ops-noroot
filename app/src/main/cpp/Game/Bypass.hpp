namespace bypass {

    int (*eee2)(...);
    int eee1(void* obj)
    {
        eee2(obj);
        return 1;
    }

    void init()
    {
        hook("0x19F06F8", eee1, eee2);
//        hook("0x", eee3, eee4);

    }
}