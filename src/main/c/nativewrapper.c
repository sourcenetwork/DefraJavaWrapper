#include <jni.h>
#include "libdefradb.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations
void releaseJavaNodeInitOptions(JNIEnv* env, jobject optionsObj, NodeInitOptions opts);
void releaseJavaCollectionOptions(JNIEnv* env, jobject optionsObj, CollectionOptions opts);

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
    memset(&opts, 0, sizeof(NodeInitOptions));
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

    // Identity
    jfieldID fid_identity = (*env)->GetFieldID(env, cls, "identity", "Lsource/defra/DefraIdentity;");
    jobject identityObj = (*env)->GetObjectField(env, optionsObj, fid_identity);
    if (identityObj != NULL) {
        jclass identityCls = (*env)->GetObjectClass(env, identityObj);
        jfieldID fid_ptr = (*env)->GetFieldID(env, identityCls, "ptr", "J");
        opts.identityPtr = (uintptr_t)(*env)->GetLongField(env, identityObj, fid_ptr);
    }

    return opts;
}

// Helper to convert a Java DefraCollectionOptions object to a C CollectionOptions struct
CollectionOptions convertJavaCollectionOptions(JNIEnv* env, jobject optionsObj) {
    CollectionOptions opts;
	memset(&opts, 0, sizeof(CollectionOptions));
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

void releaseJavaCollectionOptions(JNIEnv* env, jobject optionsObj, CollectionOptions opts) {
    jclass cls = (*env)->GetObjectClass(env, optionsObj);
    jfieldID fid_version = (*env)->GetFieldID(env, cls, "version", "Ljava/lang/String;");
    jfieldID fid_collectionID = (*env)->GetFieldID(env, cls, "collectionID", "Ljava/lang/String;");
    jfieldID fid_name = (*env)->GetFieldID(env, cls, "name", "Ljava/lang/String;");

    jstring versionStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_version);
    jstring collectionIDStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_collectionID);
    jstring nameStr = (jstring)(*env)->GetObjectField(env, optionsObj, fid_name);

    if (opts.version) (*env)->ReleaseStringUTFChars(env, versionStr, opts.version);
    if (opts.collectionID) (*env)->ReleaseStringUTFChars(env, collectionIDStr, opts.collectionID);
    if (opts.name) (*env)->ReleaseStringUTFChars(env, nameStr, opts.name);
}

//=============================================================================
// DefraNode JNI Functions
//=============================================================================

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
    Result res = CloseNode((uintptr_t)nodePtr);
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

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring sdlStr,
    jlong identityPtr
) {
    const char* sdlC = sdlStr ? (*env)->GetStringUTFChars(env, sdlStr, NULL) : NULL;
    Result res = AddCollection((uintptr_t)nodePtr, (char*)sdlC, (uintptr_t)identityPtr);
    if (sdlStr) (*env)->ReleaseStringUTFChars(env, sdlStr, sdlC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_DescribeCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = DescribeCollection((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_PatchCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring patchStr,
    jstring lensConfigStr,
    jlong identityPtr
) {
    const char* patchC = patchStr ? (*env)->GetStringUTFChars(env, patchStr, NULL) : NULL;
    const char* lensConfigC = lensConfigStr ? (*env)->GetStringUTFChars(env, lensConfigStr, NULL) : NULL;
    Result res = PatchCollection((uintptr_t)nodePtr, (char*)patchC, (char*)lensConfigC, (uintptr_t)identityPtr);
    if (patchStr) (*env)->ReleaseStringUTFChars(env, patchStr, patchC);
    if (lensConfigStr) (*env)->ReleaseStringUTFChars(env, lensConfigStr, lensConfigC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_SetActiveCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = SetActiveCollection((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_TruncateCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = TruncateCollection((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring jsonStr,
    jint isEncrypted,
    jstring encryptedFieldsStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* jsonC = jsonStr ? (*env)->GetStringUTFChars(env, jsonStr, NULL) : NULL;
    const char* encryptedFieldsC = encryptedFieldsStr ? (*env)->GetStringUTFChars(env, encryptedFieldsStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = AddDocument((uintptr_t)nodePtr, (char*)jsonC, isEncrypted, (char*)encryptedFieldsC, opts, (uintptr_t)identityPtr);
    if (jsonStr) (*env)->ReleaseStringUTFChars(env, jsonStr, jsonC);
    if (encryptedFieldsStr) (*env)->ReleaseStringUTFChars(env, encryptedFieldsStr, encryptedFieldsC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_DeleteDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jstring filterStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = DeleteDocument((uintptr_t)nodePtr, (char*)docIDC, (char*)filterC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_GetDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jboolean showDeleted,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    int showDeletedC = (showDeleted == JNI_TRUE) ? 1 : 0;
    Result res = GetDocument((uintptr_t)nodePtr, (char*)docIDC, showDeletedC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_UpdateDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jstring filterStr,
    jstring updaterStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    const char* updaterC = updaterStr ? (*env)->GetStringUTFChars(env, updaterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = UpdateDocument((uintptr_t)nodePtr, (char*)docIDC, (char*)filterC, (char*)updaterC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    if (updaterStr) (*env)->ReleaseStringUTFChars(env, updaterStr, updaterC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_NewEncryptedIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jstring fieldNameStr,
    jlong identityPtr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    const char* fieldNameC = fieldNameStr ? (*env)->GetStringUTFChars(env, fieldNameStr, NULL) : NULL;
    Result res = NewEncryptedIndex((uintptr_t)nodePtr, (char*)collectionNameC, (char*)fieldNameC, (uintptr_t)identityPtr);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    if (fieldNameStr) (*env)->ReleaseStringUTFChars(env, fieldNameStr, fieldNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ListEncryptedIndexesNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jlong identityPtr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    Result res = ListEncryptedIndexes((uintptr_t)nodePtr, (char*)collectionNameC, (uintptr_t)identityPtr);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_DeleteEncryptedIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jstring fieldNameStr,
    jlong identityPtr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    const char* fieldNameC = fieldNameStr ? (*env)->GetStringUTFChars(env, fieldNameStr, NULL) : NULL;
    Result res = DeleteEncryptedIndex((uintptr_t)nodePtr, (char*)collectionNameC, (char*)fieldNameC, (uintptr_t)identityPtr);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    if (fieldNameStr) (*env)->ReleaseStringUTFChars(env, fieldNameStr, fieldNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_NewIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring indexNameStr,
    jstring fieldsStr,
    jboolean isUnique,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    const char* fieldsC = fieldsStr ? (*env)->GetStringUTFChars(env, fieldsStr, NULL) : NULL;
    int isUniqueC = (isUnique == JNI_TRUE) ? 1 : 0;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = NewIndex((uintptr_t)nodePtr, (char*)indexNameC, (char*)fieldsC, isUniqueC, opts, (uintptr_t)identityPtr);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    if (fieldsStr) (*env)->ReleaseStringUTFChars(env, fieldsStr, fieldsC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ListIndexesNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = ListIndexes((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_DeleteIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring indexNameStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = DeleteIndex((uintptr_t)nodePtr, (char*)indexNameC, opts, (uintptr_t)identityPtr);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_IdentityNewNative(
    JNIEnv* env,
    jobject thiz,
    jstring keyTypeStr
) {
    const char* keyTypeC = keyTypeStr ? (*env)->GetStringUTFChars(env, keyTypeStr, NULL) : NULL;
    NewIdentityResult res = NewIdentity((char*)keyTypeC);
    if (keyTypeStr) (*env)->ReleaseStringUTFChars(env, keyTypeStr, keyTypeC);
    return returnDefraIdentityResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_GetNodeIdentityNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr
) {
    Result res = GetNodeIdentity((uintptr_t)nodePtr);
    return returnDefraResult(env, res);
}

JNIEXPORT void JNICALL Java_source_defra_DefraNode_FreeIdentityNative(
    JNIEnv* env,
    jobject thiz,
    jlong identityPtr
) {
    FreeIdentity((uintptr_t)identityPtr);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_SetLensNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring srcStr,
    jstring dstStr,
    jstring cfgStr
) {
    const char* srcC = srcStr ? (*env)->GetStringUTFChars(env, srcStr, NULL) : NULL;
    const char* dstC = dstStr ? (*env)->GetStringUTFChars(env, dstStr, NULL) : NULL;
    const char* cfgC = cfgStr ? (*env)->GetStringUTFChars(env, cfgStr, NULL) : NULL;
    Result res = SetLens((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)srcC, (char*)dstC, (char*)cfgC);
    if (srcStr) (*env)->ReleaseStringUTFChars(env, srcStr, srcC);
    if (dstStr) (*env)->ReleaseStringUTFChars(env, dstStr, dstC);
    if (cfgStr) (*env)->ReleaseStringUTFChars(env, cfgStr, cfgC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddLensNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr,
    jstring cfgStr
) {
    const char* cfgC = cfgStr ? (*env)->GetStringUTFChars(env, cfgStr, NULL) : NULL;
    Result res = AddLens((uintptr_t)nodePtr, (uintptr_t)identityPtr, (char*)cfgC);
    if (cfgStr) (*env)->ReleaseStringUTFChars(env, cfgStr, cfgC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ListLensesNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ListLenses((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_BlockVerifySignatureNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring keyTypeStr,
    jstring publicKeyStr,
    jstring cidStr,
    jlong identityPtr
) {
    const char* keyTypeC = keyTypeStr ? (*env)->GetStringUTFChars(env, keyTypeStr, NULL) : NULL;
    const char* publicKeyC = publicKeyStr ? (*env)->GetStringUTFChars(env, publicKeyStr, NULL) : NULL;
    const char* cidC = cidStr ? (*env)->GetStringUTFChars(env, cidStr, NULL) : NULL;
    Result res = VerifyBlockSignature((uintptr_t)nodePtr, (char*)keyTypeC, (char*)publicKeyC, (char*)cidC, (uintptr_t)identityPtr);
    if (keyTypeStr) (*env)->ReleaseStringUTFChars(env, keyTypeStr, keyTypeC);
    if (publicKeyStr) (*env)->ReleaseStringUTFChars(env, publicKeyStr, publicKeyC);
    if (cidStr) (*env)->ReleaseStringUTFChars(env, cidStr, cidC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_GetP2PInfoNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = GetP2PInfo((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ListP2PActivePeersNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ListP2PActivePeers((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ListP2PReplicatorsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ListP2PReplicators((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddP2PReplicatorNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jstring addressesStr,
    jlong identityPtr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    const char* addressesC = addressesStr ? (*env)->GetStringUTFChars(env, addressesStr, NULL) : NULL;
    Result res = AddP2PReplicator((uintptr_t)nodePtr, (char*)collectionsC, (char*)addressesC, (uintptr_t)identityPtr);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    if (addressesStr) (*env)->ReleaseStringUTFChars(env, addressesStr, addressesC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_DeleteP2PReplicatorNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jstring idStr,
    jlong identityPtr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    const char* idC = idStr ? (*env)->GetStringUTFChars(env, idStr, NULL) : NULL;
    Result res = DeleteP2PReplicator((uintptr_t)nodePtr, (char*)collectionsC, (char*)idC, (uintptr_t)identityPtr);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    if (idStr) (*env)->ReleaseStringUTFChars(env, idStr, idC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddP2PCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jlong identityPtr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = AddP2PCollection((uintptr_t)nodePtr, (char*)collectionsC, (uintptr_t)identityPtr);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_DeleteP2PCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jlong identityPtr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = DeleteP2PCollection((uintptr_t)nodePtr, (char*)collectionsC, (uintptr_t)identityPtr);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ListP2PCollectionsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ListP2PCollections((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddP2PDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jlong identityPtr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = AddP2PDocument((uintptr_t)nodePtr, (char*)collectionsC, (uintptr_t)identityPtr);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_DeleteP2PDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jlong identityPtr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    Result res = DeleteP2PDocument((uintptr_t)nodePtr, (char*)collectionsC, (uintptr_t)identityPtr);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ListP2PDocumentsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ListP2PDocuments((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_SyncP2PDocumentsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionStr,
    jstring docIDsStr,
    jstring timeoutStr,
    jlong identityPtr
) {
    const char* collectionC = collectionStr ? (*env)->GetStringUTFChars(env, collectionStr, NULL) : NULL;
    const char* docIDsC = docIDsStr ? (*env)->GetStringUTFChars(env, docIDsStr, NULL) : NULL;
    const char* timeoutC = timeoutStr ? (*env)->GetStringUTFChars(env, timeoutStr, NULL) : NULL;
    Result res = SyncP2PDocuments((uintptr_t)nodePtr, (char*)collectionC, (char*)docIDsC, (char*)timeoutC, (uintptr_t)identityPtr);
    if (collectionStr) (*env)->ReleaseStringUTFChars(env, collectionStr, collectionC);
    if (docIDsStr) (*env)->ReleaseStringUTFChars(env, docIDsStr, docIDsC);
    if (timeoutStr) (*env)->ReleaseStringUTFChars(env, timeoutStr, timeoutC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_SyncP2PCollectionVersionsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring versionIDsStr,
    jstring timeoutStr,
    jlong identityPtr
) {
    const char* versionIDsC = versionIDsStr ? (*env)->GetStringUTFChars(env, versionIDsStr, NULL) : NULL;
    const char* timeoutC = timeoutStr ? (*env)->GetStringUTFChars(env, timeoutStr, NULL) : NULL;
    Result res = SyncP2PCollectionVersions((uintptr_t)nodePtr, (char*)versionIDsC, (char*)timeoutC, (uintptr_t)identityPtr);
    if (versionIDsStr) (*env)->ReleaseStringUTFChars(env, versionIDsStr, versionIDsC);
    if (timeoutStr) (*env)->ReleaseStringUTFChars(env, timeoutStr, timeoutC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_SyncP2PBranchableCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionIDStr,
    jstring timeoutStr,
    jlong identityPtr
) {
    const char* collectionIDC = collectionIDStr ? (*env)->GetStringUTFChars(env, collectionIDStr, NULL) : NULL;
    const char* timeoutC = timeoutStr ? (*env)->GetStringUTFChars(env, timeoutStr, NULL) : NULL;
    Result res = SyncP2PBranchableCollection((uintptr_t)nodePtr, (char*)collectionIDC, (char*)timeoutC, (uintptr_t)identityPtr);
    if (collectionIDStr) (*env)->ReleaseStringUTFChars(env, collectionIDStr, collectionIDC);
    if (timeoutStr) (*env)->ReleaseStringUTFChars(env, timeoutStr, timeoutC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_ConnectP2PPeersNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring peerAddressesStr,
    jlong identityPtr
) {
    const char* peerAddressesC = peerAddressesStr ? (*env)->GetStringUTFChars(env, peerAddressesStr, NULL) : NULL;
    Result res = ConnectP2PPeers((uintptr_t)nodePtr, (char*)peerAddressesC, (uintptr_t)identityPtr);
    if (peerAddressesStr) (*env)->ReleaseStringUTFChars(env, peerAddressesStr, peerAddressesC);
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

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_GetVersionNative(
    JNIEnv* env,
    jobject thiz,
    jboolean flagFull,
    jboolean flagJSON
) {
    int flagFullC = (flagFull == JNI_TRUE) ? 1 : 0;
    int flagJSONC = (flagJSON == JNI_TRUE) ? 1 : 0;
    Result res = GetVersion(flagFullC, flagJSONC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_AddViewNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring queryStr,
    jstring sdlStr,
    jstring transformCIDStr,
    jlong identityPtr
) {
    const char* queryC = queryStr ? (*env)->GetStringUTFChars(env, queryStr, NULL) : NULL;
    const char* sdlC = sdlStr ? (*env)->GetStringUTFChars(env, sdlStr, NULL) : NULL;
    const char* transformCIDC = transformCIDStr ? (*env)->GetStringUTFChars(env, transformCIDStr, NULL) : NULL;
    Result res = AddView((uintptr_t)nodePtr, (char*)queryC, (char*)sdlC, (char*)transformCIDC, (uintptr_t)identityPtr);
    if (queryStr) (*env)->ReleaseStringUTFChars(env, queryStr, queryC);
    if (sdlStr) (*env)->ReleaseStringUTFChars(env, sdlStr, sdlC);
    if (transformCIDStr) (*env)->ReleaseStringUTFChars(env, transformCIDStr, transformCIDC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraNode_RefreshViewNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = RefreshView((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

//=============================================================================
// Transaction JNI Functions
//=============================================================================

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_TransactionCreateNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jboolean isConcurrent,
    jboolean isReadOnly
) {
    int isConcurrentC = (isConcurrent == JNI_TRUE) ? 1 : 0;
    int isReadOnlyC = (isReadOnly == JNI_TRUE) ? 1 : 0;
    NewTxnResult res = CreateTransaction((uintptr_t)nodePtr, isConcurrentC, isReadOnlyC);
    return returnDefraTransactionResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_TransactionCommitNative(
    JNIEnv* env,
    jobject thiz,
    jlong txnPtr
) {
    Result res = CommitTransaction((uintptr_t)txnPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT void JNICALL Java_source_defra_DefraTransaction_TransactionDiscardNative(
    JNIEnv* env,
    jobject thiz,
    jlong txnPtr
) {
    DiscardTransaction((uintptr_t)txnPtr);
}

// Transaction ACP Methods
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

// Transaction Document Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_AddDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring jsonStr,
    jint isEncrypted,
    jstring encryptedFieldsStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* jsonC = jsonStr ? (*env)->GetStringUTFChars(env, jsonStr, NULL) : NULL;
    const char* encryptedFieldsC = encryptedFieldsStr ? (*env)->GetStringUTFChars(env, encryptedFieldsStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = AddDocument((uintptr_t)nodePtr, (char*)jsonC, isEncrypted, (char*)encryptedFieldsC, opts, (uintptr_t)identityPtr);
    if (jsonStr) (*env)->ReleaseStringUTFChars(env, jsonStr, jsonC);
    if (encryptedFieldsStr) (*env)->ReleaseStringUTFChars(env, encryptedFieldsStr, encryptedFieldsC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_GetDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jboolean showDeleted,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    int showDeletedC = (showDeleted == JNI_TRUE) ? 1 : 0;
    Result res = GetDocument((uintptr_t)nodePtr, (char*)docIDC, showDeletedC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_UpdateDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jstring filterStr,
    jstring updaterStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    const char* updaterC = updaterStr ? (*env)->GetStringUTFChars(env, updaterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = UpdateDocument((uintptr_t)nodePtr, (char*)docIDC, (char*)filterC, (char*)updaterC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    if (updaterStr) (*env)->ReleaseStringUTFChars(env, updaterStr, updaterC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_DeleteDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jstring filterStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = DeleteDocument((uintptr_t)nodePtr, (char*)docIDC, (char*)filterC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

// Transaction Index Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_NewIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring indexNameStr,
    jstring fieldsStr,
    jboolean isUnique,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    const char* fieldsC = fieldsStr ? (*env)->GetStringUTFChars(env, fieldsStr, NULL) : NULL;
    int isUniqueC = (isUnique == JNI_TRUE) ? 1 : 0;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = NewIndex((uintptr_t)nodePtr, (char*)indexNameC, (char*)fieldsC, isUniqueC, opts, (uintptr_t)identityPtr);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    if (fieldsStr) (*env)->ReleaseStringUTFChars(env, fieldsStr, fieldsC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ListIndexesNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = ListIndexes((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_DeleteIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring indexNameStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = DeleteIndex((uintptr_t)nodePtr, (char*)indexNameC, opts, (uintptr_t)identityPtr);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

// Transaction P2P Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_GetP2PInfoNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = GetP2PInfo((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ListP2PReplicatorsNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jlong identityPtr
) {
    Result res = ListP2PReplicators((uintptr_t)nodePtr, (uintptr_t)identityPtr);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_AddP2PReplicatorNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionsStr,
    jstring addressesStr,
    jlong identityPtr
) {
    const char* collectionsC = collectionsStr ? (*env)->GetStringUTFChars(env, collectionsStr, NULL) : NULL;
    const char* addressesC = addressesStr ? (*env)->GetStringUTFChars(env, addressesStr, NULL) : NULL;
    Result res = AddP2PReplicator((uintptr_t)nodePtr, (char*)collectionsC, (char*)addressesC, (uintptr_t)identityPtr);
    if (collectionsStr) (*env)->ReleaseStringUTFChars(env, collectionsStr, collectionsC);
    if (addressesStr) (*env)->ReleaseStringUTFChars(env, addressesStr, addressesC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_ConnectP2PPeersNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring peerAddressesStr,
    jlong identityPtr
) {
    const char* peerAddressesC = peerAddressesStr ? (*env)->GetStringUTFChars(env, peerAddressesStr, NULL) : NULL;
    Result res = ConnectP2PPeers((uintptr_t)nodePtr, (char*)peerAddressesC, (uintptr_t)identityPtr);
    if (peerAddressesStr) (*env)->ReleaseStringUTFChars(env, peerAddressesStr, peerAddressesC);
    return returnDefraResult(env, res);
}

// Transaction Query Methods
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

// Transaction Collection Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_AddCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring sdlStr,
    jlong identityPtr
) {
    const char* sdlC = sdlStr ? (*env)->GetStringUTFChars(env, sdlStr, NULL) : NULL;
    Result res = AddCollection((uintptr_t)nodePtr, (char*)sdlC, (uintptr_t)identityPtr);
    if (sdlStr) (*env)->ReleaseStringUTFChars(env, sdlStr, sdlC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_PatchCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring patchStr,
    jstring lensConfigStr,
    jlong identityPtr
) {
    const char* patchC = patchStr ? (*env)->GetStringUTFChars(env, patchStr, NULL) : NULL;
    const char* lensConfigC = lensConfigStr ? (*env)->GetStringUTFChars(env, lensConfigStr, NULL) : NULL;
    Result res = PatchCollection((uintptr_t)nodePtr, (char*)patchC, (char*)lensConfigC, (uintptr_t)identityPtr);
    if (patchStr) (*env)->ReleaseStringUTFChars(env, patchStr, patchC);
    if (lensConfigStr) (*env)->ReleaseStringUTFChars(env, lensConfigStr, lensConfigC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_SetActiveCollectionNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = SetActiveCollection((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

// Transaction View Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_AddViewNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring queryStr,
    jstring sdlStr,
    jstring transformCIDStr,
    jlong identityPtr
) {
    const char* queryC = queryStr ? (*env)->GetStringUTFChars(env, queryStr, NULL) : NULL;
    const char* sdlC = sdlStr ? (*env)->GetStringUTFChars(env, sdlStr, NULL) : NULL;
    const char* transformCIDC = transformCIDStr ? (*env)->GetStringUTFChars(env, transformCIDStr, NULL) : NULL;
    Result res = AddView((uintptr_t)nodePtr, (char*)queryC, (char*)sdlC, (char*)transformCIDC, (uintptr_t)identityPtr);
    if (queryStr) (*env)->ReleaseStringUTFChars(env, queryStr, queryC);
    if (sdlStr) (*env)->ReleaseStringUTFChars(env, sdlStr, sdlC);
    if (transformCIDStr) (*env)->ReleaseStringUTFChars(env, transformCIDStr, transformCIDC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraTransaction_RefreshViewNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = RefreshView((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

//=============================================================================
// DefraCollection JNI Functions
//=============================================================================

// Document Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_AddDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring jsonStr,
    jint isEncrypted,
    jstring encryptedFieldsStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* jsonC = jsonStr ? (*env)->GetStringUTFChars(env, jsonStr, NULL) : NULL;
    const char* encryptedFieldsC = encryptedFieldsStr ? (*env)->GetStringUTFChars(env, encryptedFieldsStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = AddDocument((uintptr_t)nodePtr, (char*)jsonC, isEncrypted, (char*)encryptedFieldsC, opts, (uintptr_t)identityPtr);
    if (jsonStr) (*env)->ReleaseStringUTFChars(env, jsonStr, jsonC);
    if (encryptedFieldsStr) (*env)->ReleaseStringUTFChars(env, encryptedFieldsStr, encryptedFieldsC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_DeleteDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jstring filterStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = DeleteDocument((uintptr_t)nodePtr, (char*)docIDC, (char*)filterC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_GetDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jboolean showDeleted,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    int showDeletedC = (showDeleted == JNI_TRUE) ? 1 : 0;
    Result res = GetDocument((uintptr_t)nodePtr, (char*)docIDC, showDeletedC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_UpdateDocumentNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring docIDStr,
    jstring filterStr,
    jstring updaterStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* docIDC = docIDStr ? (*env)->GetStringUTFChars(env, docIDStr, NULL) : NULL;
    const char* filterC = filterStr ? (*env)->GetStringUTFChars(env, filterStr, NULL) : NULL;
    const char* updaterC = updaterStr ? (*env)->GetStringUTFChars(env, updaterStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = UpdateDocument((uintptr_t)nodePtr, (char*)docIDC, (char*)filterC, (char*)updaterC, opts, (uintptr_t)identityPtr);
    if (docIDStr) (*env)->ReleaseStringUTFChars(env, docIDStr, docIDC);
    if (filterStr) (*env)->ReleaseStringUTFChars(env, filterStr, filterC);
    if (updaterStr) (*env)->ReleaseStringUTFChars(env, updaterStr, updaterC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

// Encrypted Index Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_NewEncryptedIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jstring fieldNameStr,
    jlong identityPtr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    const char* fieldNameC = fieldNameStr ? (*env)->GetStringUTFChars(env, fieldNameStr, NULL) : NULL;
    Result res = NewEncryptedIndex((uintptr_t)nodePtr, (char*)collectionNameC, (char*)fieldNameC, (uintptr_t)identityPtr);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    if (fieldNameStr) (*env)->ReleaseStringUTFChars(env, fieldNameStr, fieldNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_ListEncryptedIndexesNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jlong identityPtr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    Result res = ListEncryptedIndexes((uintptr_t)nodePtr, (char*)collectionNameC, (uintptr_t)identityPtr);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_DeleteEncryptedIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring collectionNameStr,
    jstring fieldNameStr,
    jlong identityPtr
) {
    const char* collectionNameC = collectionNameStr ? (*env)->GetStringUTFChars(env, collectionNameStr, NULL) : NULL;
    const char* fieldNameC = fieldNameStr ? (*env)->GetStringUTFChars(env, fieldNameStr, NULL) : NULL;
    Result res = DeleteEncryptedIndex((uintptr_t)nodePtr, (char*)collectionNameC, (char*)fieldNameC, (uintptr_t)identityPtr);
    if (collectionNameStr) (*env)->ReleaseStringUTFChars(env, collectionNameStr, collectionNameC);
    if (fieldNameStr) (*env)->ReleaseStringUTFChars(env, fieldNameStr, fieldNameC);
    return returnDefraResult(env, res);
}

// Index Methods
JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_NewIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring indexNameStr,
    jstring fieldsStr,
    jboolean isUnique,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    const char* fieldsC = fieldsStr ? (*env)->GetStringUTFChars(env, fieldsStr, NULL) : NULL;
    int isUniqueC = (isUnique == JNI_TRUE) ? 1 : 0;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = NewIndex((uintptr_t)nodePtr, (char*)indexNameC, (char*)fieldsC, isUniqueC, opts, (uintptr_t)identityPtr);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    if (fieldsStr) (*env)->ReleaseStringUTFChars(env, fieldsStr, fieldsC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_ListIndexesNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jobject optionsObj,
    jlong identityPtr
) {
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = ListIndexes((uintptr_t)nodePtr, opts, (uintptr_t)identityPtr);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jobject JNICALL Java_source_defra_DefraCollection_DeleteIndexNative(
    JNIEnv* env,
    jobject thiz,
    jlong nodePtr,
    jstring indexNameStr,
    jobject optionsObj,
    jlong identityPtr
) {
    const char* indexNameC = indexNameStr ? (*env)->GetStringUTFChars(env, indexNameStr, NULL) : NULL;
    CollectionOptions opts = convertJavaCollectionOptions(env, optionsObj);
    Result res = DeleteIndex((uintptr_t)nodePtr, (char*)indexNameC, opts, (uintptr_t)identityPtr);
    if (indexNameStr) (*env)->ReleaseStringUTFChars(env, indexNameStr, indexNameC);
    releaseJavaCollectionOptions(env, optionsObj, opts);
    return returnDefraResult(env, res);
}

JNIEXPORT jboolean JNICALL
Java_source_defra_DefraBLE_handleFoundPeer(JNIEnv *env, jclass clazz, jstring remotePID) {
    const char *pid = (*env)->GetStringUTFChars(env, remotePID, NULL);
    int result = BLEHandleFoundPeer((char*)pid);
    (*env)->ReleaseStringUTFChars(env, remotePID, pid);
    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_source_defra_DefraBLE_handleLostPeer(JNIEnv *env, jclass clazz, jstring remotePID) {
    const char *pid = (*env)->GetStringUTFChars(env, remotePID, NULL);
    BLEHandleLostPeer((char*)pid);
    (*env)->ReleaseStringUTFChars(env, remotePID, pid);
}

JNIEXPORT void JNICALL
Java_source_defra_DefraBLE_receiveFromPeer(JNIEnv *env, jclass clazz, jstring remotePID, jbyteArray payload) {
    const char *pid = (*env)->GetStringUTFChars(env, remotePID, NULL);
    jsize len = (*env)->GetArrayLength(env, payload);
    jbyte *bytes = (*env)->GetByteArrayElements(env, payload, NULL);
    BLEReceiveFromPeer((char*)pid, (void*)bytes, (int)len);
    (*env)->ReleaseByteArrayElements(env, payload, bytes, JNI_ABORT);
    (*env)->ReleaseStringUTFChars(env, remotePID, pid);
}

JavaVM* gJVM = NULL;
jobject gBleInterface = NULL;

JNIEXPORT void JNICALL
Java_source_defra_DefraBLE_registerBleInterface(JNIEnv *env, jclass clazz, jobject bleInterface) {
    (*env)->GetJavaVM(env, &gJVM);
    gBleInterface = (*env)->NewGlobalRef(env, bleInterface);
}