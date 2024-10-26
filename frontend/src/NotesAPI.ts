export default class NotesAPI { // protocol specific
    constructor() {
        if (!storageAvailable("localStorage")) {
            console.log("localStorage in not connected");
        }
    }

    getAllNotes() {
        const foo = JSON.parse(localStorage.getItem("notes") || "[]");
        console.log(JSON.stringify(foo), typeof foo);

        return foo;
    }

    getNote(heroName: string): iNote {
        const zxc = localStorage.getItem("notes")!;
        console.log(zxc);
        return JSON.parse(zxc).find(((x: {heroName: string; note: iNote}) => x.heroName == heroName))   //[heroName as keyof typeof zxc];
    }

    createNote(heroName: string): boolean {
        const notes: {heroName: string; note: iNote}[] = this.getAllNotes();
        console.log(JSON.stringify(notes), typeof notes);

        if (notes.find((x: {heroName: string; note: iNote}) => x.heroName == heroName)) return false;

        const noteEntry: iNote = {
            timestamp: createTimestamp(),
            overview: "",
            tabs: [], // TODO: add here a method to check for existing tabs
        };
        notes.unshift({ heroName: heroName, note: noteEntry });
        localStorage.setItem("notes", JSON.stringify(notes));
        return true;
    }
}

// TODO: threw this interfaces into separate modules or smth
interface iTab {
    TODOcontent: string;
}

interface iNote {
    timestamp: string;
    overview: string;
    tabs: iTab[];
}

function storageAvailable(type: string) {
    let storage;
    try {
        storage = globalThis[type as keyof typeof globalThis] as Storage;
        const x = "__storage_test__";
        storage.setItem(x, x);
        storage.removeItem(x);
        return true;
    } catch (e) {
        return (
            e instanceof DOMException &&
            e.name === "QuotaExceededError" &&
            // acknowledge QuotaExceededError only if there's something already stored
            storage &&
            storage.length !== 0
        );
    }
}

function createTimestamp() {
    return (new Date()).toISOString();
}
