package source.defra;

import android.util.Log;

public class DefraNode {

    static {
        System.loadLibrary("defradb");
        System.loadLibrary("nativewrapper");
    }

    private native DefraNewNodeResult NewNodeNative(DefraNodeInitOptions options);
    private native DefraResult NodeCloseNative(long nodePtr);
    private native DefraResult ACPAddDACPolicyNative(long nodePtr, long identityPtr, String policy);
    private native DefraResult ACPAddDACActorRelationshipNative(long nodePtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDeleteDACActorRelationshipNative(long nodePtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDisableNACNative(long nodePtr, long identityPtr);
    private native DefraResult ACPReEnableNACNative(long nodePtr, long identityPtr);
    private native DefraResult ACPAddNACActorRelationshipNative(long nodePtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPDeleteNACActorRelationshipNative(long nodePtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPGetNACStatusNative(long nodePtr, long identityPtr);
    private native DefraResult AddSchemaNative(long nodePtr, String schema, long identityPtr);
    private native DefraResult BlockVerifySignatureNative(long nodePtr, String keyType, String publicKey, String cid);
    private native DefraResult CollectionCreateNative(long nodePtr, String json, boolean isEncrypted, String encryptedFields, DefraCollectionOptions options);
    private native DefraResult CollectionDeleteNative(long nodePtr, String docID, String filter, DefraCollectionOptions options);
    private native DefraResult CollectionDescribeNative(long nodePtr, DefraCollectionOptions options);
    private native DefraResult CollectionListDocIDsNative(long nodePtr, DefraCollectionOptions options);
    private native DefraResult CollectionUpdateNative(long nodePtr, String json, String filter, String updater, DefraCollectionOptions options);
    private native DefraResult CollectionGetNative(long nodePtr, String docID, boolean showDeleted, DefraCollectionOptions options);
    private native DefraResult CollectionPatchNative(long nodePtr, String patch, String lensConfig, DefraCollectionOptions options);
    private native DefraResult SetActiveCollectionNative(long nodePtr, String version);
    private native DefraIdentityResult IdentityNewNative(String keyType);
    private native DefraResult NodeIdentityNative(long nodePtr);
    private native DefraResult IndexCreateNative(long nodePtr, String collectionName, String indexName, String fields, boolean isUnique);
    private native DefraResult IndexListNative(long nodePtr, String collectionName);
    private native DefraResult IndexDropNative(long nodePtr, String collectionName, String indexName);
    private native DefraResult ExecuteQueryNative(long nodePtr, String query, long identityPtr, String operationName, String variables);
    private native DefraResult PollSubscriptionNative(String id);
    private native DefraResult CloseSubscriptionNative(String id);
    private native DefraResult VersionGetNative(boolean flagFull, boolean flagJSON);
    private native DefraResult ViewAddNative(long nodePtr, String query, String sdl, String transformStr);
    private native DefraResult ViewRefreshNative(long nodePtr, String viewNameStr, String collectionIDStr, String versionIDStr, boolean getInactive);
    private native DefraResult LensSetNative(long nodePtr, String src, String dst, String cfg);
    private native DefraResult LensDownNative(long nodePtr, String collectionID, String documents);
    private native DefraResult LensUpNative(long nodePtr, String collectionID, String documents);
    private native DefraResult LensReloadNative(long nodePtr);
    private native DefraResult LensSetRegistryNative(long nodePtr, String collectionID, String cfg);
    private native DefraResult P2PInfoNative(long nodePtr);
    private native DefraResult P2PgetAllReplicatorsNative(long nodePtr);
    private native DefraResult P2PsetReplicatorNative(long nodePtr, String collections, String peerInfo);
    private native DefraResult P2PdeleteReplicatorNative(long nodePtr, String collections, String peerInfo);
    private native DefraResult P2PcollectionAddNative(long nodePtr, String collections);
    private native DefraResult P2PcollectionRemoveNative(long nodePtr, String collections);
    private native DefraResult P2PcollectionGetAllNative(long nodePtr);
    private native DefraResult P2PdocumentAddNative(long nodePtr, String collections);
    private native DefraResult P2PdocumentRemoveNative(long nodePtr, String collections);
    private native DefraResult P2PdocumentGetAllNative(long nodePtr);
    private native DefraResult P2PdocumentSyncNative(long nodePtr, String collection, String docIDs, String timeoutStr);
    private native DefraResult P2PconnectNative(long nodePtr, String peerID, String peerAddresses);
    private native DefraTransactionResult TransactionCreateNative(long nodePtr, boolean isConcurrent, boolean isReadOnly);

    private long nodePtr;

    // Constructor will call native function to create node, and assign the pointer
    // of this object to the handle associated with the node.
    public DefraNode(DefraNodeInitOptions options) {
        DefraNewNodeResult result = NewNodeNative(options);
        if (result.status != 0) {
            this.nodePtr = 0;
            Log.e("DefraNode", "Error creating node: " + result.error);
            return;
        }
        this.nodePtr = result.nodePtr;
    }

    public DefraResult close() {
        return NodeCloseNative(this.nodePtr);
    }

    public DefraResult ACPAddDACPolicy(String policy) {
        return ACPAddDACPolicyNative(this.nodePtr, 0, policy);
    }

    public DefraResult ACPAddDACPolicy(String policy, DefraIdentity identity) {
        return ACPAddDACPolicyNative(this.nodePtr, identity.getPointer(), policy);
    }

    public DefraResult ACPAddDACActorRelationship(String collection, String docID, String relation, String actor) {
        return ACPAddDACActorRelationshipNative(this.nodePtr, 0, collection, docID, relation, actor);
    }

    public DefraResult ACPAddDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) {
        return ACPAddDACActorRelationshipNative(this.nodePtr, identity.getPointer(), collection, docID, relation, actor);
    }   

    public DefraResult ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor) {
        return ACPDeleteDACActorRelationshipNative(this.nodePtr, 0, collection, docID, relation, actor);
    }

    public DefraResult ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) {
        return ACPDeleteDACActorRelationshipNative(this.nodePtr, identity.getPointer(), collection, docID, relation, actor);
    }

    public DefraResult ACPDisableNAC() {
        return ACPDisableNACNative(this.nodePtr, 0);
    }

    public DefraResult ACPDisableNAC(DefraIdentity identity) {
        return ACPDisableNACNative(this.nodePtr, identity.getPointer());
    }

    public DefraResult ACPReEnableNAC() {
        return ACPReEnableNACNative(this.nodePtr, 0);
    }

    public DefraResult ACPReEnableNAC(DefraIdentity identity) {
        return ACPReEnableNACNative(this.nodePtr, identity.getPointer());
    }

    public DefraResult ACPAddNACActorRelationship(String relation, String actor) {
        return ACPAddNACActorRelationshipNative(this.nodePtr, 0, relation, actor);
    }

    public DefraResult ACPAddNACActorRelationship(String relation, String actor, DefraIdentity identity) {
        return ACPAddNACActorRelationshipNative(this.nodePtr, identity.getPointer(), relation, actor);
    }

    public DefraResult ACPDeleteNACActorRelationship(String relation, String actor) {
        return ACPDeleteNACActorRelationshipNative(this.nodePtr, 0, relation, actor);
    }

    public DefraResult ACPDeleteNACActorRelationship(String relation, String actor, DefraIdentity identity) {
        return ACPDeleteNACActorRelationshipNative(this.nodePtr, identity.getPointer(), relation, actor);
    }

    public DefraResult ACPGetNACStatus() {
        return ACPGetNACStatusNative(this.nodePtr, 0);
    }

    public DefraResult ACPGetNACStatus(DefraIdentity identity) {
        return ACPGetNACStatusNative(this.nodePtr, identity.getPointer());
    }

    public DefraResult addSchema(String schema) {
        return AddSchemaNative(this.nodePtr, schema, 0);
    }

    public DefraResult addSchema(String schema, DefraIdentity identity) {
        return AddSchemaNative(this.nodePtr, schema, identity.getPointer());
    }

    public DefraResult blockVerifySignature(String keyType, String publicKey, String cid) {
        return BlockVerifySignatureNative(this.nodePtr, keyType, publicKey, cid);
    }

    public DefraResult collectionCreate(String json, boolean isEncrypted, String encryptedFields, DefraCollectionOptions options) {
        return CollectionCreateNative(this.nodePtr, json, isEncrypted, encryptedFields, options);
    }

    public DefraResult collectionDelete(String docID, String filter, DefraCollectionOptions options) {
        return CollectionDeleteNative(this.nodePtr, docID, filter, options);
    }

    public DefraResult collectionDescribe(DefraCollectionOptions options) {
        return CollectionDescribeNative(this.nodePtr, options);
    }

    public DefraResult collectionListDocIDs(DefraCollectionOptions options) {
        return CollectionListDocIDsNative(this.nodePtr, options);
    }

    public DefraResult collectionGet(String docID, boolean showDeleted, DefraCollectionOptions options) {
        return CollectionGetNative(this.nodePtr, docID, showDeleted, options);
    }

    public DefraResult collectionUpdate(String json, String filter, String updater, DefraCollectionOptions options) {
        return CollectionUpdateNative(this.nodePtr, json, filter, updater, options);
    }

    public DefraResult collectionPatch(String patch, String lensConfig, DefraCollectionOptions options) {
        return CollectionPatchNative(this.nodePtr, patch, lensConfig, options);
    }

    public DefraResult setActiveCollection(String version) {
        return SetActiveCollectionNative(this.nodePtr, version);
    }

    public DefraIdentity identityNew(String keyType) {
        DefraIdentityResult identResult = IdentityNewNative(keyType);
        if (identResult.status != 0) {
            Log.e("DefraNode", "Error creating identity: " + identResult.error);
            return new DefraIdentity(0);
        }
        return new DefraIdentity(identResult.identityPtr);
    }

    public DefraResult nodeIdentity() {
        return NodeIdentityNative(this.nodePtr);
    }

    public DefraResult indexCreate(String collectionName, String indexName, String fields, boolean isUnique) {
        return IndexCreateNative(this.nodePtr, collectionName, indexName, fields, isUnique);
    }

    public DefraResult indexList(String collectionName) {
        return IndexListNative(this.nodePtr, collectionName);
    }

    public DefraResult indexDrop(String collectionName, String indexName) {
        return IndexDropNative(this.nodePtr, collectionName, indexName);
    }

    public DefraResult executeQuery(String query, String operationName, String variables) {
        return ExecuteQueryNative(this.nodePtr, query, 0, operationName, variables);
    }

    public DefraResult executeQuery(String query, String operationName, String variables, DefraIdentity identity) {
        return ExecuteQueryNative(this.nodePtr, query, identity.getPointer(), operationName, variables);
    }

    public DefraResult pollSubscription(String id) {
        return PollSubscriptionNative(id);
    }

    public DefraResult closeSubscription(String id) {
        return CloseSubscriptionNative(id);
    }

    public DefraResult versionGet(boolean flagFull, boolean flagJSON) {
        return VersionGetNative(flagFull, flagJSON);
    }

    public DefraResult viewAdd(String query, String sdl, String transformStr) {
        return ViewAddNative(this.nodePtr, query, sdl, transformStr);
    }

    public DefraResult viewRefresh(String viewNameStr, String collectionIDStr, String versionIDStr, boolean getInactive) {
        return ViewRefreshNative(this.nodePtr, viewNameStr, collectionIDStr, versionIDStr, getInactive);
    }

    public DefraResult lensSet(String src, String dst, String cfg) {
        return LensSetNative(this.nodePtr, src, dst, cfg);
    }

    public DefraResult lensDown(String collectionID, String documents) {
        return LensDownNative(this.nodePtr, collectionID, documents);
    }

    public DefraResult lensUp(String collectionID, String documents) {
        return LensUpNative(this.nodePtr, collectionID, documents);
    }

    public DefraResult lensReload() {
        return LensReloadNative(this.nodePtr);
    }

    public DefraResult lensSetRegistry(String collectionID, String cfg) {
        return LensSetRegistryNative(this.nodePtr, collectionID, cfg);
    }

    public DefraResult P2PInfo() {
        return P2PInfoNative(this.nodePtr);
    }

    public DefraResult P2PgetAllReplicators() {
        return P2PgetAllReplicatorsNative(this.nodePtr);
    }

    public DefraResult P2PsetReplicator(String collections, String peerInfo) {
        return P2PsetReplicatorNative(this.nodePtr, collections, peerInfo);
    }

    public DefraResult P2PdeleteReplicator(String collections, String peerInfo) {
        return P2PdeleteReplicatorNative(this.nodePtr, collections, peerInfo);
    }

    public DefraResult P2PcollectionAdd(String collections) {
        return P2PcollectionAddNative(this.nodePtr, collections);
    }

    public DefraResult P2PcollectionRemove(String collections) {
        return P2PcollectionRemoveNative(this.nodePtr, collections);
    }

    public DefraResult P2PcollectionGetAll() {
        return P2PcollectionGetAllNative(this.nodePtr);
    }

    public DefraResult P2PdocumentAdd(String collections) {
        return P2PdocumentAddNative(this.nodePtr, collections);
    }

    public DefraResult P2PdocumentRemove(String collections) {
        return P2PdocumentRemoveNative(this.nodePtr, collections);
    }

    public DefraResult P2PdocumentGetAll() {
        return P2PdocumentGetAllNative(this.nodePtr);
    }

    public DefraResult P2PdocumentSync(String collection, String docIDs, String timeoutStr) {
        return P2PdocumentSyncNative(this.nodePtr, collection, docIDs, timeoutStr);
    }

    public DefraResult P2Pconnect(String peerID, String peerAddresses) {
        return P2PconnectNative(this.nodePtr, peerID, peerAddresses);
    }

    public DefraTransaction transactionCreate(boolean isConcurrent, boolean isReadOnly) {
        DefraTransactionResult txnResult = TransactionCreateNative(this.nodePtr, isConcurrent, isReadOnly);
        if (txnResult.status != 0) {
            Log.e("DefraNode", "Error creating transaction: " + txnResult.error);
            return new DefraTransaction(0);
        }
        return new DefraTransaction(txnResult.txnPtr);
    }

    public long getPointer() {
        return this.nodePtr;
    }
    
}