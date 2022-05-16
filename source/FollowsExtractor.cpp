#include "FollowsExtractor.h"

// FollowsT
std::unordered_set<int> FollowsExtractor::computeAllFollowed(int stmt) {
    // 1. Get statements that come before; ie statements that the current stmt follow.
    //    1.1 Current stmt pulls the "followed" from prev stmt (ie the statement that current stmt
    //        directly follows) and adds prev stmt to it.
    //    1.2 If current stmt does not have a prev stmt, then it leaves followedList as empty.

    int followed = 0; // default no followed
    std::unordered_set<int> allFollowed;

    if (PKB::stmtHasFollowed(stmt)) {
        followed = PKB::getFollowedOfStmt(stmt);
        allFollowed = PKB::getFollowedListOfStmt(followed);

        // add current followed
        allFollowed.insert(followed); // last added element is the stmt directly followed

        return allFollowed;

    }
    else {
        // no stmt comes before
        // ensure this is empty for induction to be true
        assert(PKB::getFollowedListOfStmt(stmt).size() == 0);

        return PKB::getFollowedListOfStmt(stmt); // by default empty set
    }
}

std::unordered_set<int> FollowsExtractor::computeAllFollowers(int stmt) {
    // 1. Get statements that come after; ie statements that follow the current stmt
    //    1.1 Current stmt pulls the "followers" from next stmt (ie the statement that directly follows
    //        the current stmt) and adds next stmt to it.
    //    1.2 If current stmt does not have a next stmt, then it leaves followerList as empty.

    int follower = 0; // default no follower
    std::unordered_set<int> allFollowers;

    if (PKB::stmtHasFollower(stmt)) {
        follower = PKB::getFollowerOfStmt(stmt);
        allFollowers = PKB::getFollowerListOfStmt(follower); // list of stmts that follow "follower"

        // add current followed
        allFollowers.insert(follower); // last added element is the stmt directly followed

        return allFollowers;

    }
    else {
        // no stmt comes before
        // ensure this is empty for induction to be true
        assert(PKB::getFollowerListOfStmt(stmt).size() == 0);

        return PKB::getFollowerListOfStmt(stmt); // by default empty set
    }
}