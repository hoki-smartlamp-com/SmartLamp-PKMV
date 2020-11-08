import firebase from "firebase/app";
import "firebase/database";

// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyDQxdRAi3rFg9nC0GlH3ZTl5lvI-sc7y18",
  authDomain: "hoki-smartlamp-c540f.firebaseapp.com",
  databaseURL: "https://hoki-smartlamp-c540f.firebaseio.com",
  projectId: "hoki-smartlamp-c540f",
  storageBucket: "hoki-smartlamp-c540f.appspot.com",
  messagingSenderId: "668618462752",
  appId: "1:668618462752:web:0a8ca0143a4f463fc08e45",
  measurementId: "G-LQ1ZQWXVR5",
};
// Initialize Firebase
const firebaseApp = firebase.initializeApp(firebaseConfig);

export const db = firebase.database();
export default firebaseApp;
