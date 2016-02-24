#ifndef PVIF_H
#define PVIF_H

#include <map>

#include <dbChannel.h>

#include <pv/bitSet.h>
#include <pv/pvData.h>

struct DBCH {
    dbChannel *chan;
    DBCH() :chan(0) {}
    explicit DBCH(dbChannel *ch);
    explicit DBCH(const char *name);
    ~DBCH();

    void swap(DBCH&);

    operator dbChannel*() { return chan; }
    dbChannel *operator->() { return chan; }
private:
    DBCH(const DBCH&);
    DBCH& operator=(const DBCH&);
};

struct PVIF {
    PVIF(dbChannel *ch, const epics::pvData::PVStructurePtr& p);
    virtual ~PVIF() {}

    dbChannel *chan;
    epics::pvData::PVStructurePtr pvalue;

    //! Copy from PDB record to pvalue (call dbChannelGet())
    //! caller must lock record
    virtual void put(epics::pvData::BitSet& mask, unsigned dbe, db_field_log *pfl) =0;
    //! Copy from pvalue to PDB record (call dbChannelPut())
    //! caller must lock record
    virtual void get(epics::pvData::BitSet& mask) =0;

    static void Init();

    // fetch the structure description for a DBR type
    static epics::pvData::StructureConstPtr dtype(dbChannel *chan);

    // Create a PVIF associating the given channel to the given PVStructure node (may not be actual root)
    static PVIF* attach(dbChannel* ch, const epics::pvData::PVStructurePtr& root);
};

#endif // PVIF_H