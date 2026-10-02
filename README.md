# WanderStay - Full Stack Accommodation Marketplace

A dynamic vacation rental and property listing web platform built using the **MEN stack** (MongoDB, Express.js, Node.js). 

This project was developed as a comprehensive **academic capstone project** to bridge theoretical software engineering concepts with practical, production-level web development patterns.

## Project Overview

**WanderStay** simulates a full featured real world marketplace where users can discover, create, update, and review property listings. The platform enforces strict authentication and authorization boundaries, integrates interactive geolocation mapping via Mapbox, and handles dynamic image management and persistent session tracking.

---

## What This Project Taught Me

Building WanderStay from scratch served as a deep dive into full stack architecture, API design, and system security. Key technical takeaways include:

### 1. Robust Server Side Architecture (MVC Pattern)
* Designed a modular Model View Controller (**MVC**) architecture to cleanly decouple database models, application routing, and user interface rendering.
* Leveraged **Express Router** to build RESTful endpoints for multi resource CRUD operations (`/listings`, `/reviews`, `/users`).

### 2. Authentication & Authorization Security
* Implemented local authentication flows using **Passport.js** and hashed salted credentials via `passport-local-mongoose`.
* Authored custom Express middleware to enforce authorization checks (e.g., verifying listing ownership so only the creator can edit or delete a listing, and protecting sensitive routes against unauthenticated users).

### 3. Database Modeling & Relational Integrity in NoSQL
* Designed schemas using **Mongoose** with document referencing (`ObjectId`) to connect reviews, users, and listings.
* Implemented Mongoose middleware hooks (such as cascading deletions to clean up associated reviews when a parent listing is removed).
* Handled schema level validation using **Joi** to sanitize incoming request bodies and prevent malformed data from hitting the database.

### 4. Session & State Management
* Managed cross-request state and temporary UI notifications using `express-session` and `connect-flash`.
* Configured persistent session storage backed by **MongoDB (`connect-mongo`)** to prevent memory leaks and session loss on server restarts.

### 5. Third-Party API Integration & Media Handling
* Integrated **Mapbox Geocoding & Maps SDK** to convert human-readable addresses into geographic coordinates and render interactive maps for each property.
* Managed environment configurations (`dotenv`) to keep sensitive credentials, API tokens, and database secrets out of source control.

### 6. Production Grade Error Handling
* Built custom centralized error classes extending `Error` and implemented an asynchronous wrapper utility (`wrapAsync`) to catch unhandled promise rejections cleanly without crashing the Node.js event loop.

---

## Tech Stack

* **Backend:** Node.js, Express.js
* **Database:** MongoDB, Mongoose ODM
* **Templating Engine:** EJS, EJS-Mate
* **Authentication:** Passport.js, Passport-Local
* **Validation:** Joi
* **APIs & Services:** Mapbox Geocoding API
* **Session Management:** Express-Session, Connect-Mongo
