if(process.env.NODE_ENV != "production"){
    require("dotenv").config();
}

const express = require("express");
const app = express();
const mongoose = require("mongoose");
const Listing = require("./models/listing.js");
const path = require("path");
const methodOverride = require("method-override");
ejsMate = require("ejs-mate");
const wrapAsync = require("./utils/wrapAsync.js");
const ExpressError = require("./utils/ExpressError.js"); 
const {listingSchema, reviewSchema} = require("./schema.js");
const Review = require("./models/reviews.js");

const listingsRouter = require("./routes/listing.js");
const reviewsRouter = require("./routes/review.js");
const session = require("express-session");
const MongoStore = require("connect-mongo").default;
console.log("DEBUG: MongoStore value:", MongoStore);
const flash = require("connect-flash");
const passport = require("passport");
const LocalStrategy = require("passport-local");
const User = require("./models/user.js");
const userRouter = require("./routes/user.js");

// const MONGO_URL = "mongodb://127.0.0.1:27017/wanderlust";

const dbUrl = process.env.ATLASDB_URL;

main()
    .then(() => {
        console.log("connected to DB");
    })
    .catch((err) => {
        console.log("error occured");
    });

async function main(){
    await mongoose.connect(dbUrl);
}

app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));
app.use(express.urlencoded({extended: true}));
app.use(methodOverride("_method"));
app.engine("ejs", ejsMate);                                       
app.use(express.static(path.join(__dirname, "/public")));

const store = MongoStore.create({
    mongoUrl: dbUrl,
    crypto: {
        secret: process.env.SECRET,
    },
    touchAfter: 24*3600,
});

store.on("error", (err) => {
    console.log("ERROR in Mongo Session Store", err);
});

const sessionOptions = {
    store,
    secret: process.env.SECRET,
    resave: false,
    saveUninitialized: true,
    cookie: {
        expires: Date.now()+ 7*24*60*60*1000,
        maxAge: 7*24*60*60*1000,
        httpOnly: true,
    },
};



app.use(session(sessionOptions));
app.use(flash());

app.use(passport.initialize());

app.use(passport.session());

passport.use(new LocalStrategy(User.authenticate()));

passport.serializeUser(User.serializeUser());
passport.deserializeUser(User.deserializeUser());

// app.get("/", (req, res) => {
//     res.send("Hi, I am root");
// });

app.use((req, res, next) => {
    res.locals.success = req.flash("success");
    res.locals.error = req.flash("error");
    res.locals.currUser = req.user;
    next();
});

app.get("/demouser", async(req, res) => {
    let fakeUser = new User({
        email: "student@gmail.com",
        username: "delta-student"
    });

    let registeredUser = await User.register(fakeUser,"helloworld");
    res.send(registeredUser);
});


app.use("/listings", listingsRouter);
app.use("/listings/:id/reviews", reviewsRouter);
app.use("/", userRouter);

const validateReview = (req, res, next) => {
    let {error} = reviewSchema.validate(req.body);
    if(error){
        let errMsg = error.details.map((el) => el.message).join(",");
        throw new ExpressError(400, errMsg);
    }else{
        next();
    }
};



//Create route
// app.post("/listings",validateListing, wrapAsync(async (req, res, next) => {
    // let {title, description, image, price, location, country} = req.body;
    //changing this because we have another and a simple way
    //made changes in the new.ejs file for accessing these values
        // const newListing = new Listing(req.body.listing);
//         newListing.image = { url: req.body.listing.image, filename: "some_default" };
//         await newListing.save();
//         res.redirect("/listings");
// })
// );


// app.get("/testListing", async (req, res) => {
//     let sampleListing = new Listing({
//         title: "My New Villa",
//         description: "By the beach",
//         price: 1200,
//         location: "Calangute, Goa", 
//         country: "India",
//     });

//     await sampleListing.save();
//     console.log("sample was saved");
//     res.send("Successfull testing");
// });


// app.all("*", (req, res, next) => {
//     next(new ExpressError(404, "Page Not Found!"));
// });


app.use((err, req, res, next) => {
    console.log("--- ERROR STACK ---"); // Add this
    console.log(err); // Add this
    let {statusCode = 500, message = "Something went wrong!"} = err;
    res.status(statusCode).render("error.ejs", {message});
    // res.status(statusCode).send(message);
    // res.send("Something went wrong");
});

app.listen(8080, () => {
    console.log("Server is listening");
});

