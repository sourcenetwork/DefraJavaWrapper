package source.defra;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;

public class DefraNode {

    static {
        NativeLoader.load("nativewrapper");
        NativeLoader.load("defradb");
    }

    private native DefraNewNodeResult NewNodeNative(DefraNodeInitOptions options);
    private native DefraResult NodeCloseNative(long nodePtr);
    
    // ACP Methods
    private native DefraResult ACPAddDACPolicyNative(long nodePtr, long identityPtr, String policy);
    private native DefraResult ACPAddDACActorRelationshipNative(long nodePtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDeleteDACActorRelationshipNative(long nodePtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDisableNACNative(long nodePtr, long identityPtr);
    private native DefraResult ACPReEnableNACNative(long nodePtr, long identityPtr);
    private native DefraResult ACPAddNACActorRelationshipNative(long nodePtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPDeleteNACActorRelationshipNative(long nodePtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPGetNACStatusNative(long nodePtr, long identityPtr);
    
    // Collection Methods
    private native DefraResult AddCollectionNative(long nodePtr, String sdl, long identityPtr);
    private native DefraResult DescribeCollectionNative(long nodePtr, DefraCollectionOptions options, long identityPtr);
    private native DefraResult PatchCollectionNative(long nodePtr, String patch, String lensConfig, long identityPtr);
    private native DefraResult SetActiveCollectionNative(long nodePtr, DefraCollectionOptions options, long identityPtr);
    private native DefraResult TruncateCollectionNative(long nodePtr, DefraCollectionOptions options, long identityPtr);
    
    // Document Methods
    private native DefraResult AddDocumentNative(long nodePtr, String json, int isEncrypted, String encryptedFields, DefraCollectionOptions options, long identityPtr);
    private native DefraResult DeleteDocumentNative(long nodePtr, String docID, String filter, DefraCollectionOptions options, long identityPtr);
    private native DefraResult GetDocumentNative(long nodePtr, String docID, boolean showDeleted, DefraCollectionOptions options, long identityPtr);
    private native DefraResult UpdateDocumentNative(long nodePtr, String docID, String filter, String updater, DefraCollectionOptions options, long identityPtr);
    
    // Encrypted Index Methods
    private native DefraResult NewEncryptedIndexNative(long nodePtr, String collectionName, String fieldName, long identityPtr);
    private native DefraResult ListEncryptedIndexesNative(long nodePtr, String collectionName, long identityPtr);
    private native DefraResult DeleteEncryptedIndexNative(long nodePtr, String collectionName, String fieldName, long identityPtr);
    
    // Index Methods
    private native DefraResult NewIndexNative(long nodePtr, String indexName, String fields, boolean isUnique, DefraCollectionOptions options, long identityPtr);
    private native DefraResult ListIndexesNative(long nodePtr, DefraCollectionOptions options, long identityPtr);
    private native DefraResult DeleteIndexNative(long nodePtr, String indexName, DefraCollectionOptions options, long identityPtr);
    
    // Identity Methods
    private native DefraIdentityResult IdentityNewNative(String keyType);
    private native DefraResult GetNodeIdentityNative(long nodePtr);
    private native void FreeIdentityNative(long identityPtr);
    
    // Lens Methods
    private native DefraResult SetLensNative(long nodePtr, long identityPtr, String src, String dst, String cfg);
    private native DefraResult AddLensNative(long nodePtr, long identityPtr, String cfg);
    private native DefraResult ListLensesNative(long nodePtr, long identityPtr);
    
    // P2P Methods
    private native DefraResult GetP2PInfoNative(long nodePtr, long identityPtr);
    private native DefraResult ListP2PActivePeersNative(long nodePtr, long identityPtr);
    private native DefraResult ListP2PReplicatorsNative(long nodePtr, long identityPtr);
    private native DefraResult AddP2PReplicatorNative(long nodePtr, String collections, String addresses, long identityPtr);
    private native DefraResult DeleteP2PReplicatorNative(long nodePtr, String collections, String id, long identityPtr);
    private native DefraResult AddP2PCollectionNative(long nodePtr, String collections, long identityPtr);
    private native DefraResult DeleteP2PCollectionNative(long nodePtr, String collections, long identityPtr);
    private native DefraResult ListP2PCollectionsNative(long nodePtr, long identityPtr);
    private native DefraResult AddP2PDocumentNative(long nodePtr, String collections, long identityPtr);
    private native DefraResult DeleteP2PDocumentNative(long nodePtr, String collections, long identityPtr);
    private native DefraResult ListP2PDocumentsNative(long nodePtr, long identityPtr);
    private native DefraResult SyncP2PDocumentsNative(long nodePtr, String collection, String docIDs, String timeout, long identityPtr);
    private native DefraResult SyncP2PCollectionVersionsNative(long nodePtr, String versionIDs, String timeout, long identityPtr);
    private native DefraResult SyncP2PBranchableCollectionNative(long nodePtr, String collectionID, String timeout, long identityPtr);
    private native DefraResult ConnectP2PPeersNative(long nodePtr, String peerAddresses, long identityPtr);
    
    // Query Methods
    private native DefraResult ExecuteQueryNative(long nodePtr, String query, long identityPtr, String operationName, String variables);
    private native DefraResult PollSubscriptionNative(String id);
    private native DefraResult CloseSubscriptionNative(String id);
    
    // Version Method
    private native DefraResult GetVersionNative(boolean flagFull, boolean flagJSON);
    
    // View Methods
    private native DefraResult AddViewNative(long nodePtr, String query, String sdl, String transformCID, long identityPtr);
    private native DefraResult RefreshViewNative(long nodePtr, DefraCollectionOptions options, long identityPtr);
    
    // Block Verification
    private native DefraResult VerifyBlockSignatureNative(long nodePtr, String keyType, String publicKey, String cid, long identityPtr);
    
    // Transaction Methods
    private native DefraTransactionResult TransactionCreateNative(long nodePtr, boolean isConcurrent, boolean isReadOnly);
    
    private long nodePtr;

    public DefraNode(DefraNodeInitOptions options) throws DefraException {
        DefraNewNodeResult result = NewNodeNative(options);
        if (result.status != 0) {
            this.nodePtr = 0;
            throw new DefraException(result.error);
        }
        this.nodePtr = result.nodePtr;
    }

    public DefraResult close() throws DefraException {
        DefraResult result = NodeCloseNative(this.nodePtr);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // ACP Methods
    public DefraResult ACPAddDACPolicy(String policy) throws DefraException {
        DefraResult result = ACPAddDACPolicyNative(this.nodePtr, 0, policy);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPAddDACPolicy(String policy, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPAddDACPolicyNative(this.nodePtr, identity.getPointer(), policy);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPAddDACActorRelationship(String collection, String docID, String relation, String actor) throws DefraException {
        DefraResult result = ACPAddDACActorRelationshipNative(this.nodePtr, 0, collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPAddDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPAddDACActorRelationshipNative(this.nodePtr, identity.getPointer(), collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }   

    public DefraResult ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor) throws DefraException {
        DefraResult result = ACPDeleteDACActorRelationshipNative(this.nodePtr, 0, collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPDeleteDACActorRelationshipNative(this.nodePtr, identity.getPointer(), collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPDisableNAC() throws DefraException {
        DefraResult result = ACPDisableNACNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPDisableNAC(DefraIdentity identity) throws DefraException {
        DefraResult result = ACPDisableNACNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPReEnableNAC() throws DefraException {
        DefraResult result = ACPReEnableNACNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPReEnableNAC(DefraIdentity identity) throws DefraException {
        DefraResult result = ACPReEnableNACNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPAddNACActorRelationship(String relation, String actor) throws DefraException {
        DefraResult result = ACPAddNACActorRelationshipNative(this.nodePtr, 0, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPAddNACActorRelationship(String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPAddNACActorRelationshipNative(this.nodePtr, identity.getPointer(), relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPDeleteNACActorRelationship(String relation, String actor) throws DefraException {
        DefraResult result = ACPDeleteNACActorRelationshipNative(this.nodePtr, 0, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPDeleteNACActorRelationship(String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPDeleteNACActorRelationshipNative(this.nodePtr, identity.getPointer(), relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPGetNACStatus() throws DefraException {
        DefraResult result = ACPGetNACStatusNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult ACPGetNACStatus(DefraIdentity identity) throws DefraException {
        DefraResult result = ACPGetNACStatusNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Collection Methods
    public DefraResult addCollection(String sdl) throws DefraException {
        DefraResult result = AddCollectionNative(this.nodePtr, sdl, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addCollection(String sdl, DefraIdentity identity) throws DefraException {
        DefraResult result = AddCollectionNative(this.nodePtr, sdl, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult describeCollection(DefraCollectionOptions options) throws DefraException {
        DefraResult result = DescribeCollectionNative(this.nodePtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Helper function for getCollectionByName functions
    private DefraCollection getCollectionByNameWithIdentityPointer(String name, long identityPtr) throws DefraException {
		DefraCollectionOptions copts = new DefraCollectionOptions();
		copts.name = name;
		DefraResult describeResult = DescribeCollectionNative(this.nodePtr, copts, identityPtr);
		if (describeResult.status != 0) {
			throw new DefraException(describeResult.error);
		}
        String collectionID = "";
        String versionID = "";
        try {
            JSONArray array = new JSONArray(describeResult.value);
            if (array.length() == 0) {
            throw new DefraException("Collection with name '" + name + "' not found");
            }
            JSONObject first = array.getJSONObject(0);
            collectionID = first.getString("CollectionID");
            versionID = first.getString("VersionID");
        } catch (JSONException e) {
            throw new DefraException(e.getMessage());
        }
		return new DefraCollection(this.nodePtr, name, collectionID, versionID);
    }
	
	public DefraCollection getCollectionByName(String name) throws DefraException {
        return getCollectionByNameWithIdentityPointer(name, 0);
	}
	
	public DefraCollection getCollectionByName(String name, DefraIdentity identity) throws DefraException {
        return getCollectionByNameWithIdentityPointer(name, identity.getPointer());
	}

    public DefraResult describeCollection(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = DescribeCollectionNative(this.nodePtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult patchCollection(String patch, String lensConfig) throws DefraException {
        DefraResult result = PatchCollectionNative(this.nodePtr, patch, lensConfig, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult patchCollection(String patch, String lensConfig, DefraIdentity identity) throws DefraException {
        DefraResult result = PatchCollectionNative(this.nodePtr, patch, lensConfig, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult setActiveCollection(DefraCollectionOptions options) throws DefraException {
        DefraResult result = SetActiveCollectionNative(this.nodePtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult setActiveCollection(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = SetActiveCollectionNative(this.nodePtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult truncateCollection(DefraCollectionOptions options) throws DefraException {
        DefraResult result = TruncateCollectionNative(this.nodePtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult truncateCollection(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = TruncateCollectionNative(this.nodePtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Document Methods
    public DefraResult addDocument(String json, boolean isEncrypted, String encryptedFields, DefraCollectionOptions options) throws DefraException {
        DefraResult result = AddDocumentNative(this.nodePtr, json, isEncrypted ? 1 : 0, encryptedFields, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addDocument(String json, boolean isEncrypted, String encryptedFields, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = AddDocumentNative(this.nodePtr, json, isEncrypted ? 1 : 0, encryptedFields, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteDocument(String docID, String filter, DefraCollectionOptions options) throws DefraException {
        DefraResult result = DeleteDocumentNative(this.nodePtr, docID, filter, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteDocument(String docID, String filter, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteDocumentNative(this.nodePtr, docID, filter, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult getDocument(String docID, boolean showDeleted, DefraCollectionOptions options) throws DefraException {
        DefraResult result = GetDocumentNative(this.nodePtr, docID, showDeleted, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult getDocument(String docID, boolean showDeleted, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = GetDocumentNative(this.nodePtr, docID, showDeleted, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult updateDocument(String docID, String filter, String updater, DefraCollectionOptions options) throws DefraException {
        DefraResult result = UpdateDocumentNative(this.nodePtr, docID, filter, updater, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult updateDocument(String docID, String filter, String updater, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = UpdateDocumentNative(this.nodePtr, docID, filter, updater, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Encrypted Index Methods
    public DefraResult newEncryptedIndex(String collectionName, String fieldName) throws DefraException {
        return NewEncryptedIndexNative(this.nodePtr, collectionName, fieldName, 0);
    }

    public DefraResult newEncryptedIndex(String collectionName, String fieldName, DefraIdentity identity) throws DefraException {
        DefraResult result = NewEncryptedIndexNative(this.nodePtr, collectionName, fieldName, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listEncryptedIndexes(String collectionName) throws DefraException {
        DefraResult result = ListEncryptedIndexesNative(this.nodePtr, collectionName, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listEncryptedIndexes(String collectionName, DefraIdentity identity) throws DefraException {
        DefraResult result = ListEncryptedIndexesNative(this.nodePtr, collectionName, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteEncryptedIndex(String collectionName, String fieldName) throws DefraException {
        DefraResult result = DeleteEncryptedIndexNative(this.nodePtr, collectionName, fieldName, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteEncryptedIndex(String collectionName, String fieldName, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteEncryptedIndexNative(this.nodePtr, collectionName, fieldName, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Index Methods
    public DefraResult newIndex(String indexName, String fields, boolean isUnique, DefraCollectionOptions options) throws DefraException {
        DefraResult result = NewIndexNative(this.nodePtr, indexName, fields, isUnique, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult newIndex(String indexName, String fields, boolean isUnique, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = NewIndexNative(this.nodePtr, indexName, fields, isUnique, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listIndexes(DefraCollectionOptions options) throws DefraException {
        DefraResult result = ListIndexesNative(this.nodePtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listIndexes(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = ListIndexesNative(this.nodePtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteIndex(String indexName, DefraCollectionOptions options) throws DefraException {
        DefraResult result = DeleteIndexNative(this.nodePtr, indexName, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteIndex(String indexName, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteIndexNative(this.nodePtr, indexName, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Identity Methods
    public DefraIdentity identityNew(String keyType) throws DefraException {
        DefraIdentityResult identResult = IdentityNewNative(keyType);
        if (identResult.status != 0) {
            throw new DefraException(identResult.error);
        }
        return new DefraIdentity(identResult.identityPtr);
    }

    public DefraResult getNodeIdentity() throws DefraException {
        DefraResult result = GetNodeIdentityNative(this.nodePtr);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public void freeIdentity(DefraIdentity identity) throws DefraException {
        FreeIdentityNative(identity.getPointer());
    }

    // Lens Methods
    public DefraResult setLens(String src, String dst, String cfg) throws DefraException {
        DefraResult result = SetLensNative(this.nodePtr, 0, src, dst, cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult setLens(String src, String dst, String cfg, DefraIdentity identity) throws DefraException {
        DefraResult result = SetLensNative(this.nodePtr, identity.getPointer(), src, dst, cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addLens(String cfg) throws DefraException {
        DefraResult result = AddLensNative(this.nodePtr, 0, cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addLens(String cfg, DefraIdentity identity) throws DefraException {
        DefraResult result = AddLensNative(this.nodePtr, identity.getPointer(), cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listLenses() throws DefraException {
        DefraResult result = ListLensesNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listLenses(DefraIdentity identity) throws DefraException {
        DefraResult result = ListLensesNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // P2P Methods
    public DefraResult getP2PInfo() throws DefraException {
        DefraResult result = GetP2PInfoNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult getP2PInfo(DefraIdentity identity) throws DefraException {
        DefraResult result = GetP2PInfoNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PActivePeers() throws DefraException {
        DefraResult result = ListP2PActivePeersNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PActivePeers(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PActivePeersNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PReplicators() throws DefraException {
        DefraResult result = ListP2PReplicatorsNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PReplicators(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PReplicatorsNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addP2PReplicator(String collections, String addresses) throws DefraException {
        DefraResult result = AddP2PReplicatorNative(this.nodePtr, collections, addresses, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addP2PReplicator(String collections, String addresses, DefraIdentity identity) throws DefraException {
        DefraResult result = AddP2PReplicatorNative(this.nodePtr, collections, addresses, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteP2PReplicator(String collections, String id) throws DefraException {
        DefraResult result = DeleteP2PReplicatorNative(this.nodePtr, collections, id, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteP2PReplicator(String collections, String id, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteP2PReplicatorNative(this.nodePtr, collections, id, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addP2PCollection(String collections) throws DefraException {
        DefraResult result = AddP2PCollectionNative(this.nodePtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addP2PCollection(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = AddP2PCollectionNative(this.nodePtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteP2PCollection(String collections) throws DefraException {
        DefraResult result = DeleteP2PCollectionNative(this.nodePtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteP2PCollection(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteP2PCollectionNative(this.nodePtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PCollections() throws DefraException {
        DefraResult result = ListP2PCollectionsNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PCollections(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PCollectionsNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addP2PDocument(String collections) throws DefraException {
        DefraResult result = AddP2PDocumentNative(this.nodePtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addP2PDocument(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = AddP2PDocumentNative(this.nodePtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteP2PDocument(String collections) throws DefraException {
        DefraResult result = DeleteP2PDocumentNative(this.nodePtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult deleteP2PDocument(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteP2PDocumentNative(this.nodePtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PDocuments() throws DefraException {
        DefraResult result = ListP2PDocumentsNative(this.nodePtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult listP2PDocuments(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PDocumentsNative(this.nodePtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult syncP2PDocuments(String collection, String docIDs, String timeout) throws DefraException {
        DefraResult result = SyncP2PDocumentsNative(this.nodePtr, collection, docIDs, timeout, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult syncP2PDocuments(String collection, String docIDs, String timeout, DefraIdentity identity) throws DefraException {
        DefraResult result = SyncP2PDocumentsNative(this.nodePtr, collection, docIDs, timeout, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult syncP2PCollectionVersions(String versionIDs, String timeout) throws DefraException {
        DefraResult result = SyncP2PCollectionVersionsNative(this.nodePtr, versionIDs, timeout, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult syncP2PCollectionVersions(String versionIDs, String timeout, DefraIdentity identity) throws DefraException {
        DefraResult result = SyncP2PCollectionVersionsNative(this.nodePtr, versionIDs, timeout, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult syncP2PBranchableCollection(String collectionID, String timeout) throws DefraException {
        DefraResult result = SyncP2PBranchableCollectionNative(this.nodePtr, collectionID, timeout, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult syncP2PBranchableCollection(String collectionID, String timeout, DefraIdentity identity) throws DefraException {
        DefraResult result = SyncP2PBranchableCollectionNative(this.nodePtr, collectionID, timeout, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult connectP2PPeers(String peerAddresses) throws DefraException {
        DefraResult result = ConnectP2PPeersNative(this.nodePtr, peerAddresses, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult connectP2PPeers(String peerAddresses, DefraIdentity identity) throws DefraException {
        DefraResult result = ConnectP2PPeersNative(this.nodePtr, peerAddresses, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Query Methods
    public DefraResult executeQuery(String query, String operationName, String variables) throws DefraException {
        DefraResult result = ExecuteQueryNative(this.nodePtr, query, 0, operationName, variables);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult executeQuery(String query, String operationName, String variables, DefraIdentity identity) throws DefraException {
        DefraResult result = ExecuteQueryNative(this.nodePtr, query, identity.getPointer(), operationName, variables);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult pollSubscription(String id) throws DefraException {
        DefraResult result = PollSubscriptionNative(id);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult closeSubscription(String id) throws DefraException {
        DefraResult result = CloseSubscriptionNative(id);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Version Method
    public DefraResult getVersion(boolean flagFull, boolean flagJSON) throws DefraException {
        DefraResult result = GetVersionNative(flagFull, flagJSON);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // View Methods
    public DefraResult addView(String query, String sdl, String transformCID) throws DefraException {
        DefraResult result = AddViewNative(this.nodePtr, query, sdl, transformCID, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult addView(String query, String sdl, String transformCID, DefraIdentity identity) throws DefraException {
        DefraResult result = AddViewNative(this.nodePtr, query, sdl, transformCID, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult refreshView(DefraCollectionOptions options) throws DefraException {
        DefraResult result = RefreshViewNative(this.nodePtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult refreshView(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = RefreshViewNative(this.nodePtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Block Verification
    public DefraResult verifyBlockSignature(String keyType, String publicKey, String cid) throws DefraException {
        DefraResult result = VerifyBlockSignatureNative(this.nodePtr, keyType, publicKey, cid, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    public DefraResult verifyBlockSignature(String keyType, String publicKey, String cid, DefraIdentity identity) throws DefraException {
        DefraResult result = VerifyBlockSignatureNative(this.nodePtr, keyType, publicKey, cid, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result;
    }

    // Function creates a new transaction
    public DefraTransaction transactionCreate(boolean isConcurrent, boolean isReadOnly) throws DefraException {
        DefraTransactionResult txnResult = TransactionCreateNative(this.nodePtr, isConcurrent, isReadOnly);
        if (txnResult.status != 0) {
            throw new DefraException(txnResult.error);
        }
        return new DefraTransaction(txnResult.txnPtr);
    }

    // Function is a getter for the node pointer.
    public long getPointer() {
        return this.nodePtr;
    }
}