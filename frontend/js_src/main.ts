import NotesAPI from "./NotesAPI.js";

console.log("program is started");

const notes = new NotesAPI;

console.log(notes.createNote("sven"));
console.log(notes.getNote("sven"));

