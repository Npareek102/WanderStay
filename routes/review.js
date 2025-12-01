const express = require("express");
const router = express.Router({mergeParams : true});
const wrapAsync = require("../utils/wrapAsync.js");
const ExpressError = require("../utils/ExpressError.js"); 
const Review = require("../models/reviews.js");
const Listing = require("C:/MajorProject/models/listing.js");
const {validateReview, isLoggedIn, isReviewAuthor} = require("../middleware.js");

const reviewController = require("../controller/reviews.js");

//Reviews POST route
router.post("/", validateReview, isLoggedIn, wrapAsync(reviewController.createReview));

//Reviews Delete route
router.delete("/:reviewId", isLoggedIn, isReviewAuthor, wrapAsync(reviewController.destroyReview));

module.exports = router;