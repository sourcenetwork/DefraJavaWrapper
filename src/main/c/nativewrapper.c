#include <jni.h>
#include "libdefradb.h"
#include <stdlib.h>
#include <string.h>

jobject returnDefraResult(JNIEnv* env, Result res) {
    jstring errorStr = res.error ? (*env)->NewStringUTF(env, res.error) : NULL;
    jstring valueStr = res.value ? (*env)->NewStringUTF(env, res.value) : NULL;
    jclass cls = (*env)->FindClass(env, "source/defra/DefraResult");
    jmethodID ctor = (*env)->GetMethodID(env, cls, "<init>", "(ILjava/lang/String;Ljava/lang/String;)V");
    jobject resultObj = (*env)->NewObject(env, cls, ctor, (jint)res.status, errorStr, valueStr);
    return resultObj;
}

jobject returnDefraNewNodeResult(JNIEnv* env, NewNodeResult res) {
    jstring errorStr = res.error ? (*env)->NewStringUTF(env, res.error) : NULL;
    jclass cls = (*env)->FindClass(env, "source/defra/DefraNewNodeResult");
    jmethodID ctor = (*env)->GetMethodID(env, cls, "<init>", "(ILjava/lang/String;J)V");
    jobject resultObj = (*env)->NewObject(env, cls, ctor, (jint)res.status, errorStr, (jlong)res.nodePtr);
    return resultObj;
}

jobject returnDefraIdentityResult(JNIEnv* env, NewIdentityResult res) {
    jstring errorStr = res.error ? (*env)->NewStringUTF(env, res.error) : NULL;
    jclass cls = (*env)->FindClass(env, "source/defra/DefraIdentityResult");
    jmethodID ctor = (*env)->GetMethodID(env, cls, "<init>", "(ILjava/lang/String;J)V");
    jobject resultObj = (*env)->NewObject(env, cls, ctor, (jint)res.status, errorStr, (jlong)res.identityPtr);
    return resultObj;
}

jobject returnDefraTransactionResult(JNIEnv* env, NewTxnResult res) {
    jstring errorStr = res.error ? (*env)->NewStringUTF(env, res.error) : NULL;
    jclass cls = (*env)->FindClass(env, "source/defra/DefraTransactionResult");
    jmethodID ctor = (*env)->GetMethodID(env, cls, "<init>", "(ILjava/lang/String;J)V");
    jobject resultObj = (*env)->NewObject(env, cls, ctor, (jint)res.status, errorStr, (jlong)res.txnPtr);
    return resultObj;
}

// Helper to convert a Java DefraNodeInitOptions object to a C NodeInitOptions struct
NodeInitOptions convertJavaNodeInitOptions(JNIEnv* env, jobject optionsObj) {
    NodeInitOptions opts;
    jclass cls = (*env)->GetObjectClass(env, optionsObj);

    // Strings
    jfieldID fid_dbPath = (*env)->GetFieldID(env, cls, "dbPath", "Ljava/lang/String;");
    jfieldID fid_listeningAddresses = (*env)->GetFieldID(env, cls, "listeningAddresses", "Ljava/lang/String;");
    jfieldID fid_replicatorRetryIntervals = (*env)->GetFieldID(env, cls, "replicatorRetryIntervals", "Ljava/lang/String;");
    jfieldID fid_peers = (*env)->GetFieldID(env, cls, "peers", "Ljava/lang/String;");

    jstring dbPathStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_dbPath);
    jstring listeningAddressesStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_listeningAddresses);
    jstring replicatorRetryIntervalsStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_replicatorRetryIntervals);
    jstring peersStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_peers);

    opts.dbPath = (*env)->GetStringUTFChars(env, dbPathStr, 0);
    opts.listeningAddresses = (*env)->GetStringUTFChars(env, listeningAddressesStr, 0);
    opts.replicatorRetryIntervals = (*env)->GetStringUTFChars(env, replicatorRetryIntervalsStr, 0);
    opts.peers = (*env)->GetStringUTFChars(env, peersStr, 0);

    // Identity pointer
    jfieldID fid_identity = (*env)->GetFieldID(env, cls, "identity", "Lsource/defra/DefraIdentity;");
    jobject identityObj = (*env)->GetObjectField(env, optionsObj, fid_identity);
    if (identityObj != NULL) {
        jclass identityCls = (*env)->GetObjectClass(env, identityObj);
        jfieldID fid_ptr = (*env)->GetFieldID(env, identityCls, "ptr", "J");
        opts.identityPtr = (uintptr_t)(*env)->GetLongField(env, identityObj, fid_ptr);
    } else {
        opts.identityPtr = 0;
    }

    // Booleans
    jfieldID fid_inMemory = (*env)->GetFieldID(env, cls, "inMemory", "Z");
    jfieldID fid_disableP2P = (*env)->GetFieldID(env, cls, "disableP2P", "Z");
    jfieldID fid_disableAPI = (*env)->GetFieldID(env, cls, "disableAPI", "Z");
    jfieldID fid_enableNodeACP = (*env)->GetFieldID(env, cls, "enableNodeACP", "Z");

    opts.inMemory = (*env)->GetBooleanField(env, optionsObj, fid_inMemory) ? 1 : 0;
    opts.disableP2P = (*env)->GetBooleanField(env, optionsObj, fid_disableP2P) ? 1 : 0;
    opts.disableAPI = (*env)->GetBooleanField(env, optionsObj, fid_disableAPI) ? 1 : 0;
    opts.enableNodeACP = (*env)->GetBooleanField(env, optionsObj, fid_enableNodeACP) ? 1 : 0;

    // Integer
    jfieldID fid_maxTransactionRetries = (*env)->GetFieldID(env, cls, "maxTransactionRetries", "I");
    opts.maxTransactionRetries = (*env)->GetIntField(env, optionsObj, fid_maxTransactionRetries);

    return opts;
}

// Helper to convert a Java DefraCollectionOptions object to a C CollectionOptions struct
CollectionOptions convertJavaCollectionOptions(JNIEnv* env, jobject optionsObj) {
    CollectionOptions opts;
    jclass cls = (*env)->GetObjectClass(env, optionsObj);

    // Strings
    jfieldID fid_version = (*env)->GetFieldID(env, cls, "version", "Ljava/lang/String;");
    jfieldID fid_collectionID = (*env)->GetFieldID(env, cls, "collectionID", "Ljava/lang/String;");
    jfieldID fid_name = (*env)->GetFieldID(env, cls, "name", "Ljava/lang/String;");

    jstring versionStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_version);
    jstring collectionIDStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_collectionID);
    jstring nameStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_name);

    opts.version = versionStr ? (*env)->GetStringUTFChars(env, versionStr, 0) : NULL;
    opts.collectionID = collectionIDStr ? (*env)->GetStringUTFChars(env, collectionIDStr, 0) : NULL;
    opts.name = nameStr ? (*env)->GetStringUTFChars(env, nameStr, 0) : NULL;

    // Identity (via getPointer())
    jfieldID fid_identity = (*env)->GetFieldID(env, cls, "identity", "Lsource/defra/DefraIdentity;");
    jobject identityObj = (*env)->GetObjectField(env, optionsObj, fid_identity);
    if (identityObj != NULL) {
        jclass identityCls = (*env)->GetObjectClass(env, identityObj);
        jmethodID mid_getPointer = (*env)->GetMethodID(env, identityCls, "getPointer", "()J");
        if (mid_getPointer != NULL) {
            jlong ptr = (*env)->CallLongMethod(env, identityObj, mid_getPointer);
            opts.identityPtr = (uintptr_t)ptr;
        } else {
            opts.identityPtr = 0;
        }
    } else {
        opts.identityPtr = 0;
    }

    // Boolean
    jfieldID fid_getInactive = (*env)->GetFieldID(env, cls, "getInactive", "Z");
    opts.getInactive = (*env)->GetBooleanField(env, optionsObj, fid_getInactive) ? 1 : 0;

    return opts;
}


// Helper to release allocated Java strings after the call
void releaseJavaNodeInitOptions(JNIEnv* env, jobject optionsObj, NodeInitOptions opts) {
    jclass cls = (*env)->GetObjectClass(env, optionsObj);
    jfieldID fid_dbPath = (*env)->GetFieldID(env, cls, "dbPath", "Ljava/lang/String;");
    jfieldID fid_listeningAddresses = (*env)->GetFieldID(env, cls, "listeningAddresses", "Ljava/lang/String;");
    jfieldID fid_replicatorRetryIntervals = (*env)->GetFieldID(env, cls, "replicatorRetryIntervals", "Ljava/lang/String;");
    jfieldID fid_peers = (*env)->GetFieldID(env, cls, "peers", "Ljava/lang/String;");

    jstring dbPathStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_dbPath);
    jstring listeningAddressesStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_listeningAddresses);
    jstring replicatorRetryIntervalsStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_replicatorRetryIntervals);
    jstring peersStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_peers);

    (*env)->ReleaseStringUTFChars(env, dbPathStr, opts.dbPath);
    (*env)->ReleaseStringUTFChars(env, listeningAddressesStr, opts.listeningAddresses);
    (*env)->ReleaseStringUTFChars(env, replicatorRetryIntervalsStr, opts.replicatorRetryIntervals);
    (*env)->ReleaseStringUTFChars(env, peersStr, opts.peers);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_NewNodeNative
(JNIEnv *env, jobject thisObj, jobject optionsObj) {
    NodeInitOptions opts = convertJavaNodeInitOptions(env, optionsObj);
    NewNodeResult res = NewNode(opts);
    releaseJavaNodeInitOptions(env, optionsObj, opts);
    return returnDefraNewNodeResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_NodeCloseNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = NodeClose((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPAddDACPolicyNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring policyStr
) {
    const char* policyC = policyStr ? (*env)->GetStringUTFChars(env, policyStr, NULL) : NULL;
    Result res = ACPAddDACPolicy((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)policyC);
    if (policyStr) (*env)->ReleaseStringUTFChars(env, policyStr, policyC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPAddDACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring collectionStr,
    jstring docIDStr,
    jstring relationStr,
    jstring actorStr
) {
    const char* collectionC = collectionStr ? (*env)->GetStringUTFChars(env, collectionStr, NULL) : NULL;
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPAddDACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)collectionC, (char*)docIDC, (char*)relationC, (char*)actorC);
    if (collectionStr) (*env)->ReleaseStringUTFChars(env, collectionStr, collectionC);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPDeleteDACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring collectionStr,
    jstring docIDStr,
    jstring relationStr,
    jstring actorStr
) {
    const char* collectionC = collectionStr ? (*env)->GetStringUTFChars(env, collectionStr, NULL) : NULL;
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPDeleteDACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)collectionC, (char*)docIDC, (char*)relationC, (char*)actorC);
    if (collectionStr) (*env)->ReleaseStringUTFChars(env, collectionStr, collectionC);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPDisableNACNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ACPDisableNAC((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPReEnableNACNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ACPReEnableNAC((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPAddNACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring relationStr,
    jstring actorStr
) {
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPAddNACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)relationC, (char*)actorC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPDeleteNACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring relationStr,
    jstring actorStr
) {
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPDeleteNACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)relationC, (char*)actorC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ACPGetNACStatusNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ACPGetNACStatus((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddSchemaNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring schemaStr,
    jlong identityPtr
) {
    const char* schemaC = schemaStr ? (*env)->GetStringUTFChars(env, schemaStr, NULL) : NULL;
    Result res = AddSchema((uintptr_t)nodePtr, (char*)schemaC, (uintptr_t)identityPtr);
    if (schemaStr) (*env)->ReleaseStringUTFChars(env, schemaStr, schemaC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_BlockVerifySignatureNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring keyTypeStr,
    jstring publicKeyStr,
    jstring cidStr
) {
    const char* keyTypeC = keyTypeStr ? (*env)->GetStringUTFChars(env, keyTypeStr, NULL) : NULL;
    const char* publicKeyC = publicKeyStr ? (*env)->GetStringUTFChars(env, publicKeyStr, NULL) : NULL;
    const char* cidC = cidStr ? (*env)->GetStringUTFChars(env, cidStr, NULL) : NULL;
    Result res = BlockVerifySignature((uintptr_t)nodePtr, (char*)keyTypeC, (char*)publicKeyC, (char*)cidC);
    if (keyTypeStr) (*env)->ReleaseStringUTFChars(env, keyTypeStr, keyTypeC);
    if (publicKeyStr) (*env)->ReleaseStringUTFChars(env, publicKeyStr, publicKeyC);
    if (cidStr) (*env)->ReleaseStringUTFChars(env, cidStr, cidC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CollectionCreateNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring jsonStr,
    jint isEncrypted,
    jstring encryptedFieldsStr,
    jobject optionsObj
) {
    const char* jsonC = jsonStr ? (*env)->GetStringUTFChars(env, jsonStr, NULL) : NULL;
    const char* encryptedFieldsC = encryptedFieldsStr ? (*env)->GetStringUTFChars(env, encryptedFieldsStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);  
    Result res = CollectionCreate((uintptr_t)nodePtr, (char*)jsonC, isEncrypted, (char*)encryptedFieldsC, opts);
    if (jsonStr) (*env)->ReleaseStringUTFChars(env, jsonStr, jsonC);
    if (encryptedFieldsStr) (*env)->ReleaseStringUTFChars(env, encryptedFieldsStr, encryptedFieldsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CollectionDeleteNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jstring filterStr,
    jobject optionsObj
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = CollectionDelete((uintptr_t)nodePtr, (char*)docIDC, (char*)filterC, opts);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CollectionDescribeNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = CollectionDescribe((uintptr_t)nodePtr, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CollectionListDocIDsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = CollectionListDocIDs((uintptr_t)nodePtr, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CollectionUpdateNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring jsonStr,
    jstring filterStr,
    jstring updaterStr,
    jobject optionsObj
) {
    const char* jsonC = jsonStr ? (*env)->GetStringUTFChars(env, jsonStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    const char* updaterC = updaterStr ? (*env)->GetStringUTFChars(env, updaterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = CollectionUpdate((uintptr_t)nodePtr, (char*)jsonC, (char*)filterC, (char*)updaterC, opts);
    if (jsonStr) (*env)->ReleaseStringUTFChars(env, jsonStr, jsonC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    if (updaterStr) (*env)->ReleaseStringUTFChars(env, updaterStr, updaterC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CollectionGetNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jboolean showDeleted,
    jobject optionsObj
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    int showDeletedC = (showDeleted == JNI_TRUE) ? 1 : 0;
    Result res = CollectionGet((uintptr_t)nodePtr, (char*)docIDC, showDeletedC, opts);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CollectionPatchNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring patchStr,
    jstring lensConfigStr,
    jobject optionsObj
) {
    const char* patchC = patchStr ? (*env)->GetStringUTFChars(env, patchStr, NULL) : NULL;
    const char* lensConfigC = lensConfigStr ? (*env)->GetStringUTFChars(env, lensConfigStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = CollectionPatch((uintptr_t)nodePtr, (char*)patchC, (char*)lensConfigC, opts);
    if (patchStr) (*env)->ReleaseStringUTFChars(env, patchStr, patchC);
    if (lensConfigStr) (*env)->ReleaseStringUTFChars(env, lensConfigStr, lensConfigC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_SetActiveCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring versionStr
) {
    const char* versionC = versionStr ? (*env)->GetStringUTFChars(env, versionStr, NULL) : NULL;
    Result res = SetActiveCollection((uintptr_t)nodePtr, (char*)versionC);
    if (versionStr) (*env)->ReleaseStringUTFChars(env, versionStr, versionC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_IdentityNewNative(
    JNIEnv* env,
    jobject thiz,
    jstring keyTypeStr
) {
    const char* keyTypeC = keyTypeStr ? (*env)->GetStringUTFChars(env, keyTypeStr, NULL) : NULL;
    NewIdentityResult res = IdentityNew((char*)keyTypeC);
    if (keyTypeStr) (*env)->ReleaseStringUTFChars(env, keyTypeStr, keyTypeC);
    return returnDefraIdentityResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_NodeIdentityNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = NodeIdentity((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_IndexCreateNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jstring indexNameStr,
    jstring fieldsStr,
    jboolean isUnique
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    const char* fieldsC = fieldsStr ? (*env)->GetStringUTFChars(env, fieldsStr, NULL) : NULL;
    int isUniqueC = (isUnique == JNI_TRUE) ? 1 : 0;
    Result res = IndexCreate((uintptr_t)nodePtr, (char*)collectionNameC, (char*)indexNameC, (char*)fieldsC, isUniqueC);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    if (fieldsStr) (*env)->ReleaseStringUTFChars(env, fieldsStr, fieldsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_IndexListNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    Result res = IndexList((uintptr_t)nodePtr, (char*)collectionNameC);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_IndexDropNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jstring indexNameStr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    Result res = IndexDrop((uintptr_t)nodePtr, (char*)collectionNameC, (char*)indexNameC);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ExecuteQueryNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring queryStr,
    jlong identityPtr,
    jstring operationNameStr,
    jstring variablesStr
) {
    const char* queryC = queryStr ? (*env)->GetStringUTFChars(env, queryStr, NULL) : NULL;
    const char* operationNameC = operationNameStr ? (*env)->GetStringUTFChars(env, operationNameStr, NULL) : NULL;
    const char* variablesC = variablesStr ? (*env)->GetStringUTFChars(env, variablesStr, NULL) : NULL;
    Result res = ExecuteQuery((uintptr_t)nodePtr, (char*)queryC, (uintptr_t)identityPtr, (char*)operationNameC, (char*)variablesC);
    if (queryStr) (*env)->ReleaseStringUTFChars(env, queryStr, queryC);
    if (operationNameStr) (*env)->ReleaseStringUTFChars(env, operationNameStr, operationNameC);
    if (variablesStr) (*env)->ReleaseStringUTFChars(env, variablesStr, variablesC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_PollSubscriptionNative(
    JNIEnv* env,
    jobject thiz,
    jstring idStr
) {
    const char* idC = idStr ? (*env)->GetStringUTFChars(env, idStr, NULL) : NULL;
    Result res = PollSubscription((char*)idC);
    if (idStr) (*env)->ReleaseStringUTFChars(env, idStr, idC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_CloseSubscriptionNative(
    JNIEnv* env,
    jobject thiz,
    jstring idStr
) {
    const char* idC = idStr ? (*env)->GetStringUTFChars(env, idStr, NULL) : NULL;
    Result res = CloseSubscription((char*)idC);
    if (idStr) (*env)->ReleaseStringUTFChars(env, idStr, idC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_VersionGetNative(
    JNIEnv* env,
    jobject thiz,
    jboolean flagFull,
    jboolean flagJSON
) {
    int flagFullC = (flagFull == JNI_TRUE) ? 1 : 0;
    int flagJSONC = (flagJSON == JNI_TRUE) ? 1 : 0;
    Result res = VersionGet(flagFullC, flagJSONC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ViewAddNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring queryStr,
    jstring sdlStr,
    jstring transformStr
) {
    const char* queryC = queryStr ? (*env)->GetStringUTFChars(env, queryStr, NULL) : NULL;
    const char* sdlC = sdlStr ? (*env)->GetStringUTFChars(env, sdlStr, NULL) : NULL;
    const char* transformC = transformStr ? (*env)->GetStringUTFChars(env, transformStr, NULL) : NULL;
    if (queryStr) (*env)->ReleaseStringUTFChars(env, queryStr, queryC);
    if (sdlStr) (*env)->ReleaseStringUTFChars(env, sdlStr, sdlC);
    if (transformStr) (*env)->ReleaseStringUTFChars(env, transformStr, transformC);
    Result res = ViewAdd((uintptr_t)nodePtr, (char*)queryC, (char*)sdlC, (char*)transformC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ViewRefreshNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring viewNameStr,
    jstring collectionIDStr,
    jstring versionIDStr,
    jboolean getInactive
) {
    const char* viewNameC = viewNameStr ? (*env)->GetStringUTFChars(env, viewNameStr, NULL) : NULL;
    const char* collectionIDC = collectionIDStr ? (*env)->GetStringUTFChars(env, collectionIDStr, NULL) : NULL;
    const char* versionIDC = versionIDStr ? (*env)->GetStringUTFChars(env, versionIDStr, NULL) : NULL;
    int getInactiveC = (getInactive == JNI_TRUE) ? 1 : 0;
    Result res = ViewRefresh((uintptr_t)nodePtr, (char*)viewNameC, (char*)collectionIDC, (char*)versionIDC, getInactiveC);
    if (viewNameStr) (*env)->ReleaseStringUTFChars(env, viewNameStr, viewNameC);
    if (collectionIDStr) (*env)->ReleaseStringUTFChars(env, collectionIDStr, collectionIDC);
    if (versionIDStr) (*env)->ReleaseStringUTFChars(env, versionIDStr, versionIDC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_LensSetNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring srcStr,
    jstring dstStr,
    jstring cfgStr
) {
    const char* srcC = srcStr ? (*env)->GetStringUTFChars(env, srcStr, NULL) : NULL;
    const char* dstC = dstStr ? (*env)->GetStringUTFChars(env, dstStr, NULL) : NULL;
    const char* cfgC = cfgStr ? (*env)->GetStringUTFChars(env, cfgStr, NULL) : NULL;
    Result res = LensSet((uintptr_t)nodePtr, (char*)srcC, (char*)dstC, (char*)cfgC);
    if (dstStr) (*env)->ReleaseStringUTFChars(env, dstStr, dstC);
    if (cfgStr) (*env)->ReleaseStringUTFChars(env, cfgStr, cfgC);
    if (srcStr) (*env)->ReleaseStringUTFChars(env, srcStr, srcC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_LensDownNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionIDStr,
    jstring documentsStr
) {
    const char* collectionIDC = collectionIDStr ? (*env)->GetStringUTFChars(env, collectionIDStr, NULL) : NULL;
    const char* documentsC = documentsStr ? (*env)->GetStringUTFChars(env, documentsStr, NULL) : NULL;
    Result res = LensDown((uintptr_t)nodePtr, (char*)collectionIDC, (char*)documentsC);
    if (collectionIDStr) (*env)->ReleaseStringUTFChars(env, collectionIDStr, collectionIDC);
    if (documentsStr) (*env)->ReleaseStringUTFChars(env, documentsStr, documentsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_LensUpNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionIDStr,
    jstring documentsStr
) {
    const char* collectionIDC = collectionIDStr ? (*env)->GetStringUTFChars(env, collectionIDStr, NULL) : NULL;
    const char* documentsC = documentsStr ? (*env)->GetStringUTFChars(env, documentsStr, NULL) : NULL;
    Result res = LensUp((uintptr_t)nodePtr, (char*)collectionIDC, (char*)documentsC);
    if (collectionIDStr) (*env)->ReleaseStringUTFChars(env, collectionIDStr, collectionIDC);
    if (documentsStr) (*env)->ReleaseStringUTFChars(env, documentsStr, documentsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_LensReloadNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = LensReload((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_LensSetRegistryNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionIDStr,
    jstring cfgStr
) {
    const char* collectionIDC = collectionIDStr ? (*env)->GetStringUTFChars(env, collectionIDStr, NULL) : NULL;
    const char* cfgC = cfgStr ? (*env)->GetStringUTFChars(env, cfgStr, NULL) : NULL;
    Result res = LensSetRegistry((uintptr_t)nodePtr, (char*)collectionIDC, (char*)cfgC);
    if (collectionIDStr) (*env)->ReleaseStringUTFChars(env, collectionIDStr, collectionIDC);
    if (cfgStr) (*env)->ReleaseStringUTFChars(env, cfgStr, cfgC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PInfoNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PInfo((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PgetAllReplicatorsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PgetAllReplicators((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PsetReplicatorNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jstring peerInfoStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    const char* peerInfoC = peerInfoStr ? (*env)->GetStringUTFChars(env, peerInfoStr, NULL) : NULL;
    Result res = P2PsetReplicator((uintptr_t)nodePtr, (char*)collectionsC, (char*)peerInfoC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    if (peerInfoStr) (*env)->ReleaseStringUTFChars(env, peerInfoStr, peerInfoC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PdeleteReplicatorNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jstring peerInfoStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    const char* peerInfoC = peerInfoStr ? (*env)->GetStringUTFChars(env, peerInfoStr, NULL) : NULL;
    Result res = P2PdeleteReplicator((uintptr_t)nodePtr, (char*)collectionsC, (char*)peerInfoC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    if (peerInfoStr) (*env)->ReleaseStringUTFChars(env, peerInfoStr, peerInfoC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PcollectionAddNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PcollectionAdd((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PcollectionRemoveNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PcollectionRemove((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PcollectionGetAllNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PcollectionGetAll((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PdocumentAddNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PdocumentAdd((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PdocumentRemoveNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PdocumentRemove((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PdocumentGetAllNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PdocumentGetAll((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PdocumentSyncNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionStr,
    jstring docIDsStr,
    jstring timeoutStr
) {
    const char* collectionC = collectionStr ? (*env)->GetStringUTFChars(env, collectionStr, NULL) : NULL;
    const char* docIDsC = docIDsStr ? (*env)->GetStringUTFChars(env, docIDsStr, NULL) : NULL;
    const char* timeoutC = timeoutStr ? (*env)->GetStringUTFChars(env, timeoutStr, NULL) : NULL;
    Result res = P2PdocumentSync((uintptr_t)nodePtr, (char*)collectionC, (char*)docIDsC, (char*)timeoutC);
    if (collectionStr) (*env)->ReleaseStringUTFChars(env, collectionStr, collectionC);
    if (docIDsStr) (*env)->ReleaseStringUTFChars(env, docIDsStr, docIDsC);
    if (timeoutStr) (*env)->ReleaseStringUTFChars(env, timeoutStr, timeoutC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_P2PconnectNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring peerIDStr,
    jstring peerAddressesStr
) {
    const char* peerIDC = peerIDStr ? (*env)->GetStringUTFChars(env, peerIDStr, NULL) : NULL;
    const char* peerAddressesC = peerAddressesStr ? (*env)->GetStringUTFChars(env, peerAddressesStr, NULL) : NULL;
    Result res = P2Pconnect((uintptr_t)nodePtr, (char*)peerIDC, (char*)peerAddressesC);
    if (peerIDStr) (*env)->ReleaseStringUTFChars(env, peerIDStr, peerIDC);
    if (peerAddressesStr) (*env)->ReleaseStringUTFChars(env, peerAddressesStr, peerAddressesC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_TransactionCreateNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jboolean isConcurrent,
    jboolean isReadOnly
) {
    int isConcurrentC = (isConcurrent == JNI_TRUE) ? 1 : 0;
    int isReadOnlyC = (isReadOnly == JNI_TRUE) ? 1 : 0;
    NewTxnResult res = TransactionCreate((uintptr_t)nodePtr, isConcurrentC, isReadOnlyC);
    return returnDefraTransactionResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_TransactionCommitNative(
    JNIEnv* env,
    jobject thiz,
    jlong txnPtr
) {
    Result res = TransactionCommit((uintptr_t)txnPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT void JNICALL Java_source_defra_DefraTransaction_TransactionDiscardNative(
    JNIEnv* env,
    jobject thiz,
    jlong txnPtr
) {
    TransactionDiscard((uintptr_t)txnPtr);
}

// Transaction JNI Methods

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPAddDACPolicyNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring policyStr
) {
    const char* policyC = policyStr ? (*env)->GetStringUTFChars(env, policyStr, NULL) : NULL;
    Result res = ACPAddDACPolicy((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)policyC);
    if (policyStr) (*env)->ReleaseStringUTFChars(env, policyStr, policyC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPAddDACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring collectionStr,
    jstring docIDStr,
    jstring relationStr,
    jstring actorStr
) {
    const char* collectionC = collectionStr ? (*env)->GetStringUTFChars(env, collectionStr, NULL) : NULL;
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPAddDACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)collectionC, (char*)docIDC, (char*)relationC, (char*)actorC);
    if (collectionStr) (*env)->ReleaseStringUTFChars(env, collectionStr, collectionC);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPDeleteDACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring collectionStr,
    jstring docIDStr,
    jstring relationStr,
    jstring actorStr
) {
    const char* collectionC = collectionStr ? (*env)->GetStringUTFChars(env, collectionStr, NULL) : NULL;
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPDeleteDACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)collectionC, (char*)docIDC, (char*)relationC, (char*)actorC);
    if (collectionStr) (*env)->ReleaseStringUTFChars(env, collectionStr, collectionC);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPDisableNACNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ACPDisableNAC((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPReEnableNACNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ACPReEnableNAC((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPAddNACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring relationStr,
    jstring actorStr
) {
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPAddNACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)relationC, (char*)actorC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPDeleteNACActorRelationshipNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring relationStr,
    jstring actorStr
) {
    const char* relationC = relationStr ? (*env)->GetStringUTFChars(env, relationStr, NULL) : NULL;
    const char* actorC = actorStr ? (*env)->GetStringUTFChars(env, actorStr, NULL) : NULL;
    Result res = ACPDeleteNACActorRelationship((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)relationC, (char*)actorC);
    if (relationStr) (*env)->ReleaseStringUTFChars(env, relationStr, relationC);
    if (actorStr) (*env)->ReleaseStringUTFChars(env, actorStr, actorC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ACPGetNACStatusNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ACPGetNACStatus((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PInfoNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PInfo((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PgetAllReplicatorsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PgetAllReplicators((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PsetReplicatorNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jstring peerInfoStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    const char* peerInfoC = peerInfoStr ? (*env)->GetStringUTFChars(env, peerInfoStr, NULL) : NULL;
    Result res = P2PsetReplicator((uintptr_t)nodePtr, (char*)collectionsC, (char*)peerInfoC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    if (peerInfoStr) (*env)->ReleaseStringUTFChars(env, peerInfoStr, peerInfoC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PdeleteReplicatorNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jstring peerInfoStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    const char* peerInfoC = peerInfoStr ? (*env)->GetStringUTFChars(env, peerInfoStr, NULL) : NULL;
    Result res = P2PdeleteReplicator((uintptr_t)nodePtr, (char*)collectionsC, (char*)peerInfoC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    if (peerInfoStr) (*env)->ReleaseStringUTFChars(env, peerInfoStr, peerInfoC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PcollectionAddNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PcollectionAdd((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PcollectionRemoveNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PcollectionRemove((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PcollectionGetAllNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PcollectionGetAll((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PdocumentAddNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PdocumentAdd((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PdocumentRemoveNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = P2PdocumentRemove((uintptr_t)nodePtr, (char*)collectionsC);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PdocumentGetAllNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = P2PdocumentGetAll((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PdocumentSyncNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionStr,
    jstring docIDsStr,
    jstring timeoutStr
) {
    const char* collectionC = collectionStr ? (*env)->GetStringUTFChars(env, collectionStr, NULL) : NULL;
    const char* docIDsC = docIDsStr ? (*env)->GetStringUTFChars(env, docIDsStr, NULL) : NULL;
    const char* timeoutC = timeoutStr ? (*env)->GetStringUTFChars(env, timeoutStr, NULL) : NULL;
    Result res = P2PdocumentSync((uintptr_t)nodePtr, (char*)collectionC, (char*)docIDsC, (char*)timeoutC);
    if (collectionStr) (*env)->ReleaseStringUTFChars(env, collectionStr, collectionC);
    if (docIDsStr) (*env)->ReleaseStringUTFChars(env, docIDsStr, docIDsC);
    if (timeoutStr) (*env)->ReleaseStringUTFChars(env, timeoutStr, timeoutC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_P2PconnectNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring peerIDStr,
    jstring peerAddressesStr
) {
    const char* peerIDC = peerIDStr ? (*env)->GetStringUTFChars(env, peerIDStr, NULL) : NULL;
    const char* peerAddressesC = peerAddressesStr ? (*env)->GetStringUTFChars(env, peerAddressesStr, NULL) : NULL;
    Result res = P2Pconnect((uintptr_t)nodePtr, (char*)peerIDC, (char*)peerAddressesC);
    if (peerIDStr) (*env)->ReleaseStringUTFChars(env, peerIDStr, peerIDC);
    if (peerAddressesStr) (*env)->ReleaseStringUTFChars(env, peerAddressesStr, peerAddressesC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_NodeIdentityNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = NodeIdentity((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_BlockVerifySignatureNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring keyTypeStr,
    jstring publicKeyStr,
    jstring cidStr
) {
    const char* keyTypeC = keyTypeStr ? (*env)->GetStringUTFChars(env, keyTypeStr, NULL) : NULL;
    const char* publicKeyC = publicKeyStr ? (*env)->GetStringUTFChars(env, publicKeyStr, NULL) : NULL;
    const char* cidC = cidStr ? (*env)->GetStringUTFChars(env, cidStr, NULL) : NULL;
    Result res = BlockVerifySignature((uintptr_t)nodePtr, (char*)keyTypeC, (char*)publicKeyC, (char*)cidC);
    if (keyTypeStr) (*env)->ReleaseStringUTFChars(env, keyTypeStr, keyTypeC);
    if (publicKeyStr) (*env)->ReleaseStringUTFChars(env, publicKeyStr, publicKeyC);
    if (cidStr) (*env)->ReleaseStringUTFChars(env, cidStr, cidC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_AddSchemaNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring schemaStr,
    jlong identityPtr
) {
    const char* schemaC = schemaStr ? (*env)->GetStringUTFChars(env, schemaStr, NULL) : NULL;
    Result res = AddSchema((uintptr_t)nodePtr, (char*)schemaC, (uintptr_t)identityPtr);
    if (schemaStr) (*env)->ReleaseStringUTFChars(env, schemaStr, schemaC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_CollectionPatchNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring patchStr,
    jstring lensConfigStr,
    jobject optionsObj
) {
    const char* patchC = patchStr ? (*env)->GetStringUTFChars(env, patchStr, NULL) : NULL;
    const char* lensConfigC = lensConfigStr ? (*env)->GetStringUTFChars(env, lensConfigStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = CollectionPatch((uintptr_t)nodePtr, (char*)patchC, (char*)lensConfigC, opts);
    if (patchStr) (*env)->ReleaseStringUTFChars(env, patchStr, patchC);
    if (lensConfigStr) (*env)->ReleaseStringUTFChars(env, lensConfigStr, lensConfigC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_SetActiveCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring versionStr
) {
    const char* versionC = versionStr ? (*env)->GetStringUTFChars(env, versionStr, NULL) : NULL;
    Result res = SetActiveCollection((uintptr_t)nodePtr, (char*)versionC);
    if (versionStr) (*env)->ReleaseStringUTFChars(env, versionStr, versionC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ViewAddNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring queryStr,
    jstring sdlStr,
    jstring transformStr
) {
    const char* queryC = queryStr ? (*env)->GetStringUTFChars(env, queryStr, NULL) : NULL;
    const char* sdlC = sdlStr ? (*env)->GetStringUTFChars(env, sdlStr, NULL) : NULL;
    const char* transformC = transformStr ? (*env)->GetStringUTFChars(env, transformStr, NULL) : NULL;
    if (queryStr) (*env)->ReleaseStringUTFChars(env, queryStr, queryC);
    if (sdlStr) (*env)->ReleaseStringUTFChars(env, sdlStr, sdlC);
    if (transformStr) (*env)->ReleaseStringUTFChars(env, transformStr, transformC);
    Result res = ViewAdd((uintptr_t)nodePtr, (char*)queryC, (char*)sdlC, (char*)transformC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ViewRefreshNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring viewNameStr,
    jstring collectionIDStr,
    jstring versionIDStr,
    jboolean getInactive
) {
    const char* viewNameC = viewNameStr ? (*env)->GetStringUTFChars(env, viewNameStr, NULL) : NULL;
    const char* collectionIDC = collectionIDStr ? (*env)->GetStringUTFChars(env, collectionIDStr, NULL) : NULL;
    const char* versionIDC = versionIDStr ? (*env)->GetStringUTFChars(env, versionIDStr, NULL) : NULL;
    int getInactiveC = (getInactive == JNI_TRUE) ? 1 : 0;
    Result res = ViewRefresh((uintptr_t)nodePtr, (char*)viewNameC, (char*)collectionIDC, (char*)versionIDC, getInactiveC);
    if (viewNameStr) (*env)->ReleaseStringUTFChars(env, viewNameStr, viewNameC);
    if (collectionIDStr) (*env)->ReleaseStringUTFChars(env, collectionIDStr, collectionIDC);
    if (versionIDStr) (*env)->ReleaseStringUTFChars(env, versionIDStr, versionIDC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_LensSetNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring srcStr,
    jstring dstStr,
    jstring cfgStr
) {
    const char* srcC = srcStr ? (*env)->GetStringUTFChars(env, srcStr, NULL) : NULL;
    const char* dstC = dstStr ? (*env)->GetStringUTFChars(env, dstStr, NULL) : NULL;
    const char* cfgC = cfgStr ? (*env)->GetStringUTFChars(env, cfgStr, NULL) : NULL;
    Result res = LensSet((uintptr_t)nodePtr, (char*)srcC, (char*)dstC, (char*)cfgC);
    if (dstStr) (*env)->ReleaseStringUTFChars(env, dstStr, dstC);
    if (cfgStr) (*env)->ReleaseStringUTFChars(env, cfgStr, cfgC);
    if (srcStr) (*env)->ReleaseStringUTFChars(env, srcStr, srcC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_CollectionGetNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jboolean showDeleted,
    jobject optionsObj
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    int showDeletedC = (showDeleted == JNI_TRUE) ? 1 : 0;
    Result res = CollectionGet((uintptr_t)nodePtr, (char*)docIDC, showDeletedC, opts);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_IndexListNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    Result res = IndexList((uintptr_t)nodePtr, (char*)collectionNameC);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ExecuteQueryNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring queryStr,
    jlong identityPtr,
    jstring operationNameStr,
    jstring variablesStr
) {
    const char* queryC = queryStr ? (*env)->GetStringUTFChars(env, queryStr, NULL) : NULL;
    const char* operationNameC = operationNameStr ? (*env)->GetStringUTFChars(env, operationNameStr, NULL) : NULL;
    const char* variablesC = variablesStr ? (*env)->GetStringUTFChars(env, variablesStr, NULL) : NULL;
    Result res = ExecuteQuery((uintptr_t)nodePtr, (char*)queryC, (uintptr_t)identityPtr, (char*)operationNameC, (char*)variablesC);
    if (queryStr) (*env)->ReleaseStringUTFChars(env, queryStr, queryC);
    if (operationNameStr) (*env)->ReleaseStringUTFChars(env, operationNameStr, operationNameC);
    if (variablesStr) (*env)->ReleaseStringUTFChars(env, variablesStr, variablesC);
    return returnDefraResult(env, res);
}