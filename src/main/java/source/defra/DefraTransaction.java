package source.defra;

public class DefraTransaction {

    static {
        System.loadLibrary("defradb");
        System.loadLibrary("nativewrapper");
    }

    private native DefraResult TransactionCommitNative(long txnPtr);
    private native void TransactionDiscardNative(long txnPtr);
    private native DefraResult ACPAddDACPolicyNative(long txnPtr, long identityPtr, String policy);
    private native DefraResult ACPAddDACActorRelationshipNative(long txnPtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDeleteDACActorRelationshipNative(long txnPtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDisableNACNative(long txnPtr, long identityPtr);
    private native DefraResult ACPReEnableNACNative(long txnPtr, long identityPtr);
    private native DefraResult ACPAddNACActorRelationshipNative(long txnPtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPDeleteNACActorRelationshipNative(long txnPtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPGetNACStatusNative(long txnPtr, long identityPtr);
    private native DefraResult P2PInfoNative(long txnPtr);
    private native DefraResult P2PgetAllReplicatorsNative(long txnPtr);
    private native DefraResult P2PsetReplicatorNative(long txnPtr, String collections, String peerInfo);
    private native DefraResult P2PdeleteReplicatorNative(long txnPtr, String collections, String peerInfo);
    private native DefraResult P2PcollectionAddNative(long txnPtr, String collections);
    private native DefraResult P2PcollectionRemoveNative(long txnPtr, String collections);
    private native DefraResult P2PcollectionGetAllNative(long txnPtr);
    private native DefraResult P2PdocumentAddNative(long txnPtr, String collections);
    private native DefraResult P2PdocumentRemoveNative(long txnPtr, String collections);
    private native DefraResult P2PdocumentGetAllNative(long txnPtr);
    private native DefraResult P2PdocumentSyncNative(long txnPtr, String collection, String docIDs, String timeoutStr);
    private native DefraResult P2PconnectNative(long txnPtr, String peerID, String peerAddresses);
    private native DefraResult NodeIdentityNative(long txnPtr);
    private native DefraResult BlockVerifySignatureNative(long txnPtr, String keyType, String publicKey, String cid);
    private native DefraResult AddSchemaNative(long txnPtr, String schema, long identityPtr);
    private native DefraResult CollectionPatchNative(long txnPtr, String patch, String lensConfig, DefraCollectionOptions options);
    private native DefraResult SetActiveCollectionNative(long txnPtr, String version);
    private native DefraResult ViewAddNative(long txnPtr, String query, String sdl, String transformStr);
    private native DefraResult ViewRefreshNative(long txnPtr, String viewNameStr, String collectionIDStr, String versionIDStr, boolean getInactive);
    private native DefraResult LensSetNative(long txnPtr, String src, String dst, String cfg);
    private native DefraResult CollectionGetNative(long txnPtr, String docID, boolean showDeleted, DefraCollectionOptions options);
    private native DefraResult IndexListNative(long txnPtr, String collectionName);
    private native DefraResult ExecuteQueryNative(long txnPtr, String query, long identityPtr, String operationName, String variables);

    private long txnPtr;

    public DefraTransaction(long ptr) {
        this.txnPtr = ptr;
    }

    public DefraResult commit() {
        return TransactionCommitNative(this.txnPtr);
    }

    public void discard() {
        TransactionDiscardNative(this.txnPtr);
    }

        public DefraResult ACPAddDACPolicy(String policy) {
        return ACPAddDACPolicyNative(this.txnPtr, 0, policy);
    }

    public DefraResult ACPAddDACPolicy(String policy, DefraIdentity identity) {
        return ACPAddDACPolicyNative(this.txnPtr, identity.getPointer(), policy);
    }

    public DefraResult ACPAddDACActorRelationship(String collection, String docID, String relation, String actor) {
        return ACPAddDACActorRelationshipNative(this.txnPtr, 0, collection, docID, relation, actor);
    }

    public DefraResult ACPAddDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) {
        return ACPAddDACActorRelationshipNative(this.txnPtr, identity.getPointer(), collection, docID, relation, actor);
    }   

    public DefraResult ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor) {
        return ACPDeleteDACActorRelationshipNative(this.txnPtr, 0, collection, docID, relation, actor);
    }

    public DefraResult ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) {
        return ACPDeleteDACActorRelationshipNative(this.txnPtr, identity.getPointer(), collection, docID, relation, actor);
    }

    public DefraResult ACPDisableNAC() {
        return ACPDisableNACNative(this.txnPtr, 0);
    }

    public DefraResult ACPDisableNAC(DefraIdentity identity) {
        return ACPDisableNACNative(this.txnPtr, identity.getPointer());
    }

    public DefraResult ACPReEnableNAC() {
        return ACPReEnableNACNative(this.txnPtr, 0);
    }

    public DefraResult ACPReEnableNAC(DefraIdentity identity) {
        return ACPReEnableNACNative(this.txnPtr, identity.getPointer());
    }

    public DefraResult ACPAddNACActorRelationship(String relation, String actor) {
        return ACPAddNACActorRelationshipNative(this.txnPtr, 0, relation, actor);
    }

    public DefraResult ACPAddNACActorRelationship(String relation, String actor, DefraIdentity identity) {
        return ACPAddNACActorRelationshipNative(this.txnPtr, identity.getPointer(), relation, actor);
    }

    public DefraResult ACPDeleteNACActorRelationship(String relation, String actor) {
        return ACPDeleteNACActorRelationshipNative(this.txnPtr, 0, relation, actor);
    }

    public DefraResult ACPDeleteNACActorRelationship(String relation, String actor, DefraIdentity identity) {
        return ACPDeleteNACActorRelationshipNative(this.txnPtr, identity.getPointer(), relation, actor);
    }

    public DefraResult ACPGetNACStatus() {
        return ACPGetNACStatusNative(this.txnPtr, 0);
    }

    public DefraResult ACPGetNACStatus(DefraIdentity identity) {
        return ACPGetNACStatusNative(this.txnPtr, identity.getPointer());
    }
    
    public DefraResult P2PInfo() {
        return P2PInfoNative(this.txnPtr);
    }

    public DefraResult P2PgetAllReplicators() {
        return P2PgetAllReplicatorsNative(this.txnPtr);
    }

    public DefraResult P2PsetReplicator(String collections, String peerInfo) {
        return P2PsetReplicatorNative(this.txnPtr, collections, peerInfo);
    }

    public DefraResult P2PdeleteReplicator(String collections, String peerInfo) {
        return P2PdeleteReplicatorNative(this.txnPtr, collections, peerInfo);
    }

    public DefraResult P2PcollectionAdd(String collections) {
        return P2PcollectionAddNative(this.txnPtr, collections);
    }

    public DefraResult P2PcollectionRemove(String collections) {
        return P2PcollectionRemoveNative(this.txnPtr, collections);
    }

    public DefraResult P2PcollectionGetAll() {
        return P2PcollectionGetAllNative(this.txnPtr);
    }

    public DefraResult P2PdocumentAdd(String collections) {
        return P2PdocumentAddNative(this.txnPtr, collections);
    }

    public DefraResult P2PdocumentRemove(String collections) {
        return P2PdocumentRemoveNative(this.txnPtr, collections);
    }

    public DefraResult P2PdocumentGetAll() {
        return P2PdocumentGetAllNative(this.txnPtr);
    }

    public DefraResult P2PdocumentSync(String collection, String docIDs, String timeoutStr) {
        return P2PdocumentSyncNative(this.txnPtr, collection, docIDs, timeoutStr);
    }

    public DefraResult P2Pconnect(String peerID, String peerAddresses) {
        return P2PconnectNative(this.txnPtr, peerID, peerAddresses);
    }

    public DefraResult nodeIdentity() {
        return NodeIdentityNative(this.txnPtr);
    }

    public DefraResult blockVerifySignature(String keyType, String publicKey, String cid) {
        return BlockVerifySignatureNative(this.txnPtr, keyType, publicKey, cid);
    }

    public DefraResult addSchema(String schema) {
        return AddSchemaNative(this.txnPtr, schema, 0);
    }

    public DefraResult collectionPatch(String patch, String lensConfig, DefraCollectionOptions options) {
        return CollectionPatchNative(this.txnPtr, patch, lensConfig, options);
    }

    public DefraResult setActiveCollection(String version) {
        return SetActiveCollectionNative(this.txnPtr, version);
    }

    public DefraResult viewAdd(String query, String sdl, String transformStr) {
        return ViewAddNative(this.txnPtr, query, sdl, transformStr);
    }

    public DefraResult viewRefresh(String viewNameStr, String collectionIDStr, String versionIDStr, boolean getInactive) {
        return ViewRefreshNative(this.txnPtr, viewNameStr, collectionIDStr, versionIDStr, getInactive);
    }

    public DefraResult lensSet(String src, String dst, String cfg) {
        return LensSetNative(this.txnPtr, src, dst, cfg);
    }

    public DefraResult collectionGet(String docID, boolean showDeleted, DefraCollectionOptions options) {
        return CollectionGetNative(this.txnPtr, docID, showDeleted, options);
    }

    public DefraResult indexList(String collectionName) {
        return IndexListNative(this.txnPtr, collectionName);
    }

    public DefraResult executeQuery(String query, String operationName, String variables) {
        return ExecuteQueryNative(this.txnPtr, query, 0, operationName, variables);
    }

    public DefraResult executeQuery(String query, String operationName, String variables, DefraIdentity identity) {
        return ExecuteQueryNative(this.txnPtr, query, identity.getPointer(), operationName, variables);
    }

}
