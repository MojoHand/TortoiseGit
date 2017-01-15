// testCOM.js - javascript file
// test script for the GitWCRev COM/Automation-object

filesystem = new ActiveXObject("Scripting.FileSystemObject");

GitWCRevObject1 = new ActiveXObject("GitWCRev.object");
GitWCRevObject2 = new ActiveXObject("GitWCRev.object");
GitWCRevObject3 = new ActiveXObject("GitWCRev.object");
GitWCRevObject4 = new ActiveXObject("GitWCRev.object");

GitWCRevObject2_1 = new ActiveXObject("GitWCRev.object");
GitWCRevObject2_2 = new ActiveXObject("GitWCRev.object");
GitWCRevObject2_3 = new ActiveXObject("GitWCRev.object");
GitWCRevObject2_4 = new ActiveXObject("GitWCRev.object");

GitWCRevObject1.GetWCInfo(filesystem.GetAbsolutePathName("."), 1, 1);
GitWCRevObject2.GetWCInfo(filesystem.GetAbsolutePathName(".."), 1, 1);
GitWCRevObject3.GetWCInfo(filesystem.GetAbsolutePathName("GitWCRev.cpp"), 1, 1);
GitWCRevObject4.GetWCInfo(filesystem.GetAbsolutePathName("..\\.."), 1, 1);

GitWCRevObject2_1.GetWCInfo2(filesystem.GetAbsolutePathName("."), 1, 1, 1);
GitWCRevObject2_2.GetWCInfo2(filesystem.GetAbsolutePathName(".."), 1, 1, 1);
GitWCRevObject2_3.GetWCInfo2(filesystem.GetAbsolutePathName("GitWCRev.cpp"), 1, 1, 1);
GitWCRevObject2_4.GetWCInfo2(filesystem.GetAbsolutePathName("..\\.."), 1, 1, 1);

wcInfoString1 = "Revision = " + GitWCRevObject1.Revision + "\nMin Revision = " + GitWCRevObject1.MinRev + "\nMax Revision = " + GitWCRevObject1.MaxRev + "\nDate = " + GitWCRevObject1.Date + "\nURL = " + GitWCRevObject1.Url + "\nAuthor = " + GitWCRevObject1.Author + "\nHasMods = " + GitWCRevObject1.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_1.HasUnversioned + "\nHasMixed = " + GitWCRevObject1.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject1.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject1.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject1.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject1.NeedsLocking + "\nIsLocked = " + GitWCRevObject1.IsLocked + "\nLockCreationDate = " + GitWCRevObject1.LockCreationDate + "\nLockOwner = " + GitWCRevObject1.LockOwner + "\nLockComment = " + GitWCRevObject1.LockComment;
wcInfoString2 = "Revision = " + GitWCRevObject2.Revision + "\nMin Revision = " + GitWCRevObject2.MinRev + "\nMax Revision = " + GitWCRevObject2.MaxRev + "\nDate = " + GitWCRevObject2.Date + "\nURL = " + GitWCRevObject2.Url + "\nAuthor = " + GitWCRevObject2.Author + "\nHasMods = " + GitWCRevObject2.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_2.HasUnversioned + "\nHasMixed = " + GitWCRevObject2.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject2.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject2.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject2.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject2.NeedsLocking + "\nIsLocked = " + GitWCRevObject2.IsLocked + "\nLockCreationDate = " + GitWCRevObject2.LockCreationDate + "\nLockOwner = " + GitWCRevObject2.LockOwner + "\nLockComment = " + GitWCRevObject2.LockComment;
wcInfoString3 = "Revision = " + GitWCRevObject3.Revision + "\nMin Revision = " + GitWCRevObject3.MinRev + "\nMax Revision = " + GitWCRevObject3.MaxRev + "\nDate = " + GitWCRevObject3.Date + "\nURL = " + GitWCRevObject3.Url + "\nAuthor = " + GitWCRevObject3.Author + "\nHasMods = " + GitWCRevObject3.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_3.HasUnversioned + "\nHasMixed = " + GitWCRevObject3.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject3.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject3.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject3.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject3.NeedsLocking + "\nIsLocked = " + GitWCRevObject3.IsLocked + "\nLockCreationDate = " + GitWCRevObject3.LockCreationDate + "\nLockOwner = " + GitWCRevObject3.LockOwner + "\nLockComment = " + GitWCRevObject3.LockComment;
wcInfoString4 = "Revision = " + GitWCRevObject4.Revision + "\nMin Revision = " + GitWCRevObject4.MinRev + "\nMax Revision = " + GitWCRevObject4.MaxRev + "\nDate = " + GitWCRevObject4.Date + "\nURL = " + GitWCRevObject4.Url + "\nAuthor = " + GitWCRevObject4.Author + "\nHasMods = " + GitWCRevObject4.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_4.HasUnversioned + "\nHasMixed = " + GitWCRevObject4.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject4.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject4.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject4.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject4.NeedsLocking + "\nIsLocked = " + GitWCRevObject4.IsLocked + "\nLockCreationDate = " + GitWCRevObject4.LockCreationDate + "\nLockOwner = " + GitWCRevObject4.LockOwner + "\nLockComment = " + GitWCRevObject4.LockComment;

WScript.Echo(wcInfoString1);
WScript.Echo(wcInfoString2);
WScript.Echo(wcInfoString3);
WScript.Echo(wcInfoString4);

wcInfoString1 = "Revision = " + GitWCRevObject2_1.Revision + "\nMin Revision = " + GitWCRevObject2_1.MinRev + "\nMax Revision = " + GitWCRevObject2_1.MaxRev + "\nDate = " + GitWCRevObject2_1.Date + "\nURL = " + GitWCRevObject2_1.Url + "\nAuthor = " + GitWCRevObject2_1.Author + "\nHasMods = " + GitWCRevObject2_1.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_1.HasUnversioned + "\nHasMixed = " + GitWCRevObject2_1.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject2_1.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject2_1.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject2_1.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject2_1.NeedsLocking + "\nIsLocked = " + GitWCRevObject2_1.IsLocked + "\nLockCreationDate = " + GitWCRevObject2_1.LockCreationDate + "\nLockOwner = " + GitWCRevObject2_1.LockOwner + "\nLockComment = " + GitWCRevObject2_1.LockComment;
wcInfoString2 = "Revision = " + GitWCRevObject2_2.Revision + "\nMin Revision = " + GitWCRevObject2_2.MinRev + "\nMax Revision = " + GitWCRevObject2_2.MaxRev + "\nDate = " + GitWCRevObject2_2.Date + "\nURL = " + GitWCRevObject2_2.Url + "\nAuthor = " + GitWCRevObject2_2.Author + "\nHasMods = " + GitWCRevObject2_2.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_2.HasUnversioned + "\nHasMixed = " + GitWCRevObject2_2.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject2_2.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject2_2.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject2_2.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject2_2.NeedsLocking + "\nIsLocked = " + GitWCRevObject2_2.IsLocked + "\nLockCreationDate = " + GitWCRevObject2_2.LockCreationDate + "\nLockOwner = " + GitWCRevObject2_2.LockOwner + "\nLockComment = " + GitWCRevObject2_2.LockComment;
wcInfoString3 = "Revision = " + GitWCRevObject2_3.Revision + "\nMin Revision = " + GitWCRevObject2_3.MinRev + "\nMax Revision = " + GitWCRevObject2_3.MaxRev + "\nDate = " + GitWCRevObject2_3.Date + "\nURL = " + GitWCRevObject2_3.Url + "\nAuthor = " + GitWCRevObject2_3.Author + "\nHasMods = " + GitWCRevObject2_3.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_3.HasUnversioned + "\nHasMixed = " + GitWCRevObject2_3.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject2_3.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject2_3.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject2_3.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject2_3.NeedsLocking + "\nIsLocked = " + GitWCRevObject2_3.IsLocked + "\nLockCreationDate = " + GitWCRevObject2_3.LockCreationDate + "\nLockOwner = " + GitWCRevObject2_3.LockOwner + "\nLockComment = " + GitWCRevObject2_3.LockComment;
wcInfoString4 = "Revision = " + GitWCRevObject2_4.Revision + "\nMin Revision = " + GitWCRevObject2_4.MinRev + "\nMax Revision = " + GitWCRevObject2_4.MaxRev + "\nDate = " + GitWCRevObject2_4.Date + "\nURL = " + GitWCRevObject2_4.Url + "\nAuthor = " + GitWCRevObject2_4.Author + "\nHasMods = " + GitWCRevObject2_4.HasModifications + "\nHasUnversioned = " + GitWCRevObject2_4.HasUnversioned + "\nHasMixed = " + GitWCRevObject2_4.HasMixedRevisions + "\nExtAllFixed = " + GitWCRevObject2_4.HaveExternalsAllFixedRevision + "\nIsTagged = " + GitWCRevObject2_4.IsWcTagged + "\nIsSvnItem = " + GitWCRevObject2_4.IsSvnItem + "\nNeedsLocking = " + GitWCRevObject2_4.NeedsLocking + "\nIsLocked = " + GitWCRevObject2_4.IsLocked + "\nLockCreationDate = " + GitWCRevObject2_4.LockCreationDate + "\nLockOwner = " + GitWCRevObject2_4.LockOwner + "\nLockComment = " + GitWCRevObject2_4.LockComment;

WScript.Echo(wcInfoString1);
WScript.Echo(wcInfoString2);
WScript.Echo(wcInfoString3);
WScript.Echo(wcInfoString4);
