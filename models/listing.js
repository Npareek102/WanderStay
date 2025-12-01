const mongoose = require("mongoose");
const Schema = mongoose.Schema; // storing our schema in this schema variable so that we dont need to call it again and again
const Review = require("./reviews.js");

const listingSchema = new Schema ({
    title: {
        type: String,
        required : true,
    }, 
    description: String,
    image:  {
        filename: String,
        url: String,
        
    }, 
    price: Number,
    location: String,
    country: String,

    reviews: [
        {
            type: Schema.Types.ObjectId,
            ref: "Review"
        }
    ],
    owner: {
        type: Schema.Types.ObjectId,
        ref: "User",
    },
    geometry: {
        type: {
            type: String,
            enum: ["Point"],
            required: true,
        },
        coordinates: {
            type: [Number],
            required: true,
        },
    },
});

// models/listing.js

listingSchema.post("findOneAndDelete", async (listing) => {

    if (listing && listing.reviews.length) {
        await Review.deleteMany({ _id: { $in: listing.reviews } });
    }
});
const Listing = mongoose.model("Listing", listingSchema);
module.exports = Listing;