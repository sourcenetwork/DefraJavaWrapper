package source.defra;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;

public class DefraTransaction {

    static {
		System.loadLibrary("nativewrapper");
        System.loadLibrary("defradb");
    }

    // Core Transaction Methods
    private native DefraResult TransactionCommitNative(long txnPtr);
    private native void TransactionDiscardNative(long txnPtr);
    
    // ACP Methods
    private native DefraResult ACPAddDACPolicyNative(long txnPtr, long identityPtr, String policy);
    private native DefraResult ACPAddDACActorRelationshipNative(long txnPtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDeleteDACActorRelationshipNative(long txnPtr, long identityPtr, String collection, String docID, String relation, String actor);
    private native DefraResult ACPDisableNACNative(long txnPtr, long identityPtr);
    private native DefraResult ACPReEnableNACNative(long txnPtr, long identityPtr);
    private native DefraResult ACPAddNACActorRelationshipNative(long txnPtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPDeleteNACActorRelationshipNative(long txnPtr, long identityPtr, String relation, String actor);
    private native DefraResult ACPGetNACStatusNative(long txnPtr, long identityPtr);
    
    // Collection Methods
    private native DefraResult AddCollectionNative(long txnPtr, String sdl, long identityPtr);
    private native DefraResult DescribeCollectionNative(long txnPtr, DefraCollectionOptions options, long identityPtr);
    private native DefraResult PatchCollectionNative(long txnPtr, String patch, String lensConfig, long identityPtr);
    private native DefraResult SetActiveCollectionNative(long txnPtr, DefraCollectionOptions options, long identityPtr);
    private native DefraResult TruncateCollectionNative(long txnPtr, DefraCollectionOptions options, long identityPtr);
    
    // Document Methods
    private native DefraResult AddDocumentNative(long txnPtr, String json, int isEncrypted, String encryptedFields, DefraCollectionOptions options, long identityPtr);
    private native DefraResult DeleteDocumentNative(long txnPtr, String docID, String filter, DefraCollectionOptions options, long identityPtr);
    private native DefraResult GetDocumentNative(long txnPtr, String docID, boolean showDeleted, DefraCollectionOptions options, long identityPtr);
    private native DefraResult UpdateDocumentNative(long txnPtr, String docID, String filter, String updater, DefraCollectionOptions options, long identityPtr);
    
    // Encrypted Index Methods
    private native DefraResult NewEncryptedIndexNative(long txnPtr, String collectionName, String fieldName, long identityPtr);
    private native DefraResult ListEncryptedIndexesNative(long txnPtr, String collectionName, long identityPtr);
    private native DefraResult DeleteEncryptedIndexNative(long txnPtr, String collectionName, String fieldName, long identityPtr);
    
    // Index Methods
    private native DefraResult NewIndexNative(long txnPtr, String indexName, String fields, boolean isUnique, DefraCollectionOptions options, long identityPtr);
    private native DefraResult ListIndexesNative(long txnPtr, DefraCollectionOptions options, long identityPtr);
    private native DefraResult DeleteIndexNative(long txnPtr, String indexName, DefraCollectionOptions options, long identityPtr);
    
    // Identity Methods
    private native DefraResult GetNodeIdentityNative(long txnPtr);
    
    // Lens Methods
    private native DefraResult SetLensNative(long txnPtr, long identityPtr, String src, String dst, String cfg);
    private native DefraResult AddLensNative(long txnPtr, long identityPtr, String cfg);
    private native DefraResult ListLensesNative(long txnPtr, long identityPtr);
    
    // P2P Methods
    private native DefraResult GetP2PInfoNative(long txnPtr, long identityPtr);
    private native DefraResult ListP2PActivePeersNative(long txnPtr, long identityPtr);
    private native DefraResult ListP2PReplicatorsNative(long txnPtr, long identityPtr);
    private native DefraResult AddP2PReplicatorNative(long txnPtr, String collections, String addresses, long identityPtr);
    private native DefraResult DeleteP2PReplicatorNative(long txnPtr, String collections, String id, long identityPtr);
    private native DefraResult AddP2PCollectionNative(long txnPtr, String collections, long identityPtr);
    private native DefraResult DeleteP2PCollectionNative(long txnPtr, String collections, long identityPtr);
    private native DefraResult ListP2PCollectionsNative(long txnPtr, long identityPtr);
    private native DefraResult AddP2PDocumentNative(long txnPtr, String collections, long identityPtr);
    private native DefraResult DeleteP2PDocumentNative(long txnPtr, String collections, long identityPtr);
    private native DefraResult ListP2PDocumentsNative(long txnPtr, long identityPtr);
    private native DefraResult SyncP2PDocumentsNative(long txnPtr, String collection, String docIDs, String timeout, long identityPtr);
    private native DefraResult SyncP2PCollectionVersionsNative(long txnPtr, String versionIDs, String timeout, long identityPtr);
    private native DefraResult SyncP2PBranchableCollectionNative(long txnPtr, String collectionID, String timeout, long identityPtr);
    private native DefraResult ConnectP2PPeersNative(long txnPtr, String peerAddresses, long identityPtr);
    
    // Query Methods
    private native DefraResult ExecuteQueryNative(long txnPtr, String query, long identityPtr, String operationName, String variables);
    
    // View Methods
    private native DefraResult AddViewNative(long txnPtr, String query, String sdl, String transformCID, long identityPtr);
    private native DefraResult RefreshViewNative(long txnPtr, DefraCollectionOptions options, long identityPtr);
    
    // Block Verification
    private native DefraResult VerifyBlockSignatureNative(long txnPtr, String keyType, String publicKey, String cid, long identityPtr);
    
    private long txnPtr;

    public DefraTransaction(long ptr) {
        this.txnPtr = ptr;
    }

    public String commit() throws DefraException {
        DefraResult result = TransactionCommitNative(this.txnPtr);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public void discard() {
        TransactionDiscardNative(this.txnPtr);
    }

    // ACP Methods
    public String ACPAddDACPolicy(String policy) throws DefraException {
        DefraResult result = ACPAddDACPolicyNative(this.txnPtr, 0, policy);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPAddDACPolicy(String policy, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPAddDACPolicyNative(this.txnPtr, identity.getPointer(), policy);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPAddDACActorRelationship(String collection, String docID, String relation, String actor) throws DefraException {
        DefraResult result = ACPAddDACActorRelationshipNative(this.txnPtr, 0, collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPAddDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPAddDACActorRelationshipNative(this.txnPtr, identity.getPointer(), collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }   

    public String ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor) throws DefraException {
        DefraResult result = ACPDeleteDACActorRelationshipNative(this.txnPtr, 0, collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPDeleteDACActorRelationship(String collection, String docID, String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPDeleteDACActorRelationshipNative(this.txnPtr, identity.getPointer(), collection, docID, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPDisableNAC() throws DefraException {
        DefraResult result = ACPDisableNACNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPDisableNAC(DefraIdentity identity) throws DefraException {
        DefraResult result = ACPDisableNACNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPReEnableNAC() throws DefraException {
        DefraResult result = ACPReEnableNACNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPReEnableNAC(DefraIdentity identity) throws DefraException {
        DefraResult result = ACPReEnableNACNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPAddNACActorRelationship(String relation, String actor) throws DefraException {
        DefraResult result = ACPAddNACActorRelationshipNative(this.txnPtr, 0, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPAddNACActorRelationship(String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPAddNACActorRelationshipNative(this.txnPtr, identity.getPointer(), relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPDeleteNACActorRelationship(String relation, String actor) throws DefraException {
        DefraResult result = ACPDeleteNACActorRelationshipNative(this.txnPtr, 0, relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPDeleteNACActorRelationship(String relation, String actor, DefraIdentity identity) throws DefraException {
        DefraResult result = ACPDeleteNACActorRelationshipNative(this.txnPtr, identity.getPointer(), relation, actor);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPGetNACStatus() throws DefraException {
        DefraResult result = ACPGetNACStatusNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String ACPGetNACStatus(DefraIdentity identity) throws DefraException {
        DefraResult result = ACPGetNACStatusNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Collection Methods
    public String addCollection(String sdl) throws DefraException {
        DefraResult result = AddCollectionNative(this.txnPtr, sdl, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addCollection(String sdl, DefraIdentity identity) throws DefraException {
        DefraResult result = AddCollectionNative(this.txnPtr, sdl, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String describeCollection(DefraCollectionOptions options) throws DefraException {
        DefraResult result = DescribeCollectionNative(this.txnPtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String describeCollection(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = DescribeCollectionNative(this.txnPtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String patchCollection(String patch, String lensConfig) throws DefraException {
        DefraResult result = PatchCollectionNative(this.txnPtr, patch, lensConfig, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String patchCollection(String patch, String lensConfig, DefraIdentity identity) throws DefraException {
        DefraResult result = PatchCollectionNative(this.txnPtr, patch, lensConfig, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String setActiveCollection(DefraCollectionOptions options) throws DefraException {
        DefraResult result = SetActiveCollectionNative(this.txnPtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String setActiveCollection(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = SetActiveCollectionNative(this.txnPtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String truncateCollection(DefraCollectionOptions options) throws DefraException {
        DefraResult result = TruncateCollectionNative(this.txnPtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String truncateCollection(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = TruncateCollectionNative(this.txnPtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

        // Helper function for getCollectionByName functions
    private DefraCollection getCollectionByNameWithIdentityPointer(String name, long identityPtr) throws DefraException {
		DefraCollectionOptions copts = new DefraCollectionOptions();
		copts.name = name;
		DefraResult describeResult = DescribeCollectionNative(this.txnPtr, copts, identityPtr);
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
		return new DefraCollection(this.txnPtr, name, collectionID, versionID);
    }
	
	public DefraCollection getCollectionByName(String name) throws DefraException {
		return getCollectionByNameWithIdentityPointer(name, 0);		
	}
	
	public DefraCollection getCollectionByName(String name, DefraIdentity identity) throws DefraException {
		return getCollectionByNameWithIdentityPointer(name, identity.getPointer());
	}

    // Document Methods
    public String addDocument(String json, boolean isEncrypted, String encryptedFields, DefraCollectionOptions options) throws DefraException {
        DefraResult result = AddDocumentNative(this.txnPtr, json, isEncrypted ? 1 : 0, encryptedFields, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addDocument(String json, boolean isEncrypted, String encryptedFields, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = AddDocumentNative(this.txnPtr, json, isEncrypted ? 1 : 0, encryptedFields, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteDocument(String docID, String filter, DefraCollectionOptions options) throws DefraException {
        DefraResult result = DeleteDocumentNative(this.txnPtr, docID, filter, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteDocument(String docID, String filter, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteDocumentNative(this.txnPtr, docID, filter, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String getDocument(String docID, boolean showDeleted, DefraCollectionOptions options) throws DefraException {
        DefraResult result = GetDocumentNative(this.txnPtr, docID, showDeleted, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String getDocument(String docID, boolean showDeleted, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = GetDocumentNative(this.txnPtr, docID, showDeleted, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String updateDocument(String docID, String filter, String updater, DefraCollectionOptions options) throws DefraException {
        DefraResult result = UpdateDocumentNative(this.txnPtr, docID, filter, updater, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String updateDocument(String docID, String filter, String updater, DefraCollectionOptions options, DefraIdentity identity) throws DefraException{
        DefraResult result = UpdateDocumentNative(this.txnPtr, docID, filter, updater, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Encrypted Index Methods
    public String newEncryptedIndex(String collectionName, String fieldName) throws DefraException {
        DefraResult result = NewEncryptedIndexNative(this.txnPtr, collectionName, fieldName, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String newEncryptedIndex(String collectionName, String fieldName, DefraIdentity identity) throws DefraException {
        DefraResult result = NewEncryptedIndexNative(this.txnPtr, collectionName, fieldName, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listEncryptedIndexes(String collectionName) throws DefraException {
        DefraResult result = ListEncryptedIndexesNative(this.txnPtr, collectionName, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listEncryptedIndexes(String collectionName, DefraIdentity identity) throws DefraException {
        DefraResult result = ListEncryptedIndexesNative(this.txnPtr, collectionName, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteEncryptedIndex(String collectionName, String fieldName) throws DefraException {
        DefraResult result = DeleteEncryptedIndexNative(this.txnPtr, collectionName, fieldName, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteEncryptedIndex(String collectionName, String fieldName, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteEncryptedIndexNative(this.txnPtr, collectionName, fieldName, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Index Methods
    public String newIndex(String indexName, String fields, boolean isUnique, DefraCollectionOptions options) throws DefraException {
        DefraResult result = NewIndexNative(this.txnPtr, indexName, fields, isUnique, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String newIndex(String indexName, String fields, boolean isUnique, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = NewIndexNative(this.txnPtr, indexName, fields, isUnique, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listIndexes(DefraCollectionOptions options) throws DefraException {
        DefraResult result = ListIndexesNative(this.txnPtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listIndexes(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = ListIndexesNative(this.txnPtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteIndex(String indexName, DefraCollectionOptions options) throws DefraException {
        DefraResult result = DeleteIndexNative(this.txnPtr, indexName, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteIndex(String indexName, DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteIndexNative(this.txnPtr, indexName, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Identity Methods
    public String getNodeIdentity() throws DefraException {
        DefraResult result = GetNodeIdentityNative(this.txnPtr);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Lens Methods
    public String setLens(String src, String dst, String cfg) throws DefraException {
        DefraResult result = SetLensNative(this.txnPtr, 0, src, dst, cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String setLens(String src, String dst, String cfg, DefraIdentity identity) throws DefraException {
        DefraResult result = SetLensNative(this.txnPtr, identity.getPointer(), src, dst, cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addLens(String cfg) throws DefraException {
        DefraResult result = AddLensNative(this.txnPtr, 0, cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addLens(String cfg, DefraIdentity identity) throws DefraException {
        DefraResult result = AddLensNative(this.txnPtr, identity.getPointer(), cfg);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listLenses() throws DefraException {
        DefraResult result = ListLensesNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listLenses(DefraIdentity identity) throws DefraException {
        DefraResult result = ListLensesNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // P2P Methods
    public String getP2PInfo() throws DefraException {
        DefraResult result = GetP2PInfoNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String getP2PInfo(DefraIdentity identity) throws DefraException {
        DefraResult result = GetP2PInfoNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PActivePeers() throws DefraException {
        DefraResult result = ListP2PActivePeersNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PActivePeers(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PActivePeersNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PReplicators() throws DefraException {
        DefraResult result = ListP2PReplicatorsNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PReplicators(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PReplicatorsNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addP2PReplicator(String collections, String addresses) throws DefraException {
        DefraResult result = AddP2PReplicatorNative(this.txnPtr, collections, addresses, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addP2PReplicator(String collections, String addresses, DefraIdentity identity) throws DefraException {
        DefraResult result = AddP2PReplicatorNative(this.txnPtr, collections, addresses, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteP2PReplicator(String collections, String id) throws DefraException {
        DefraResult result = DeleteP2PReplicatorNative(this.txnPtr, collections, id, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteP2PReplicator(String collections, String id, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteP2PReplicatorNative(this.txnPtr, collections, id, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addP2PCollection(String collections) throws DefraException {
        DefraResult result = AddP2PCollectionNative(this.txnPtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addP2PCollection(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = AddP2PCollectionNative(this.txnPtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteP2PCollection(String collections) throws DefraException {
        DefraResult result = DeleteP2PCollectionNative(this.txnPtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteP2PCollection(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteP2PCollectionNative(this.txnPtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PCollections() throws DefraException {
        DefraResult result = ListP2PCollectionsNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PCollections(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PCollectionsNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addP2PDocument(String collections) throws DefraException {
        DefraResult result = AddP2PDocumentNative(this.txnPtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addP2PDocument(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = AddP2PDocumentNative(this.txnPtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteP2PDocument(String collections) throws DefraException {
        DefraResult result = DeleteP2PDocumentNative(this.txnPtr, collections, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String deleteP2PDocument(String collections, DefraIdentity identity) throws DefraException {
        DefraResult result = DeleteP2PDocumentNative(this.txnPtr, collections, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PDocuments() throws DefraException {
        DefraResult result = ListP2PDocumentsNative(this.txnPtr, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String listP2PDocuments(DefraIdentity identity) throws DefraException {
        DefraResult result = ListP2PDocumentsNative(this.txnPtr, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String syncP2PDocuments(String collection, String docIDs, String timeout) throws DefraException {
        DefraResult result = SyncP2PDocumentsNative(this.txnPtr, collection, docIDs, timeout, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String syncP2PDocuments(String collection, String docIDs, String timeout, DefraIdentity identity) throws DefraException {
        DefraResult result = SyncP2PDocumentsNative(this.txnPtr, collection, docIDs, timeout, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String syncP2PCollectionVersions(String versionIDs, String timeout) throws DefraException {
        DefraResult result = SyncP2PCollectionVersionsNative(this.txnPtr, versionIDs, timeout, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String syncP2PCollectionVersions(String versionIDs, String timeout, DefraIdentity identity) throws DefraException {
        DefraResult result = SyncP2PCollectionVersionsNative(this.txnPtr, versionIDs, timeout, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String syncP2PBranchableCollection(String collectionID, String timeout) throws DefraException {
        DefraResult result = SyncP2PBranchableCollectionNative(this.txnPtr, collectionID, timeout, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String syncP2PBranchableCollection(String collectionID, String timeout, DefraIdentity identity) throws DefraException {
        DefraResult result = SyncP2PBranchableCollectionNative(this.txnPtr, collectionID, timeout, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String connectP2PPeers(String peerAddresses) throws DefraException {
        DefraResult result = ConnectP2PPeersNative(this.txnPtr, peerAddresses, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String connectP2PPeers(String peerAddresses, DefraIdentity identity) throws DefraException {
        DefraResult result = ConnectP2PPeersNative(this.txnPtr, peerAddresses, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Query Methods
    public String executeQuery(String query, String operationName, String variables) throws DefraException {
        DefraResult result = ExecuteQueryNative(this.txnPtr, query, 0, operationName, variables);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String executeQuery(String query, String operationName, String variables, DefraIdentity identity) throws DefraException {
        DefraResult result = ExecuteQueryNative(this.txnPtr, query, identity.getPointer(), operationName, variables);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // View Methods
    public String addView(String query, String sdl, String transformCID) throws DefraException {
        DefraResult result = AddViewNative(this.txnPtr, query, sdl, transformCID, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String addView(String query, String sdl, String transformCID, DefraIdentity identity) throws DefraException {
        DefraResult result = AddViewNative(this.txnPtr, query, sdl, transformCID, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String refreshView(DefraCollectionOptions options) throws DefraException {
        DefraResult result = RefreshViewNative(this.txnPtr, options, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String refreshView(DefraCollectionOptions options, DefraIdentity identity) throws DefraException {
        DefraResult result = RefreshViewNative(this.txnPtr, options, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Block Verification
    public String verifyBlockSignature(String keyType, String publicKey, String cid) throws DefraException {
        DefraResult result = VerifyBlockSignatureNative(this.txnPtr, keyType, publicKey, cid, 0);
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    public String verifyBlockSignature(String keyType, String publicKey, String cid, DefraIdentity identity) throws DefraException {
        DefraResult result = VerifyBlockSignatureNative(this.txnPtr, keyType, publicKey, cid, identity.getPointer());
        if (result.status != 0) {
            throw new DefraException(result.error);
        }
        return result.value;
    }

    // Function is a getter for the transaction pointer
    public long getPointer() {
        return this.txnPtr;
    }
}